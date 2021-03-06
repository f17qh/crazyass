package crazyass

import "errors"
import "time"
import "labix.org/v2/mgo"
import "labix.org/v2/mgo/bson"
import "strconv"

var ErrInvalidParam = errors.New("Invalid paramters")

func init() {
	// register login cmd
	ClientProcRegister(kCmdUserLogin, ProcUserLogin)
	ClientProcRegister(kCmdStartPlay, ProcStartPlay)
	ClientProcRegister(kCmdEndPlay, ProcEndPlay)
	ClientProcRegister(kCmdIAPAddHeart, ProcIAPAddHeart)
	ClientProcRegister(kCmdEventLock, ProcEventLock)
	ClientProcRegister(kCmdUseItem, ProcUseItem)
}

// user in memory
type User struct {
	// whether or not user is login
	ready bool
	// client info
	c *Client
	// data struct saved in db
	udb UserDb
	// whether of not user info is changed.
	// true means should sync to db
	dirty bool
}

type UserDb struct {
	UserId string `bson:"_id"`
	LastLogin uint32
	Heart uint32
	NextStage uint32
	RegTime uint32
	DeviceInfo string
	EventLock []int
}

func userRegister(u *UserDb, id string) {
	u.UserId = id
	u.Heart = 99
	u.NextStage = 1
	u.RegTime = uint32(time.Now().Unix())
	// u.LastLogin = u.RegTime
	CALog.Info("New user %s\n", id)

	// TODO: save device info
}

func (u *User) Load(userid string) error {
	// load user
	c := SharedDBSession().DB(DatabaseName).C("user")
	err := c.Find(bson.M{"_id": userid}).One(&u.udb)
	if err == mgo.ErrNotFound {
		userRegister(&u.udb, userid)
		u.dirty = true;
	} else if err != nil {
		CALog.Error("user %s error %s", userid, err.Error())
		return err
	}
	// u.udb.Heart = 99
	// u.ready = true
	return nil
}

func (u *User) Store() error {
	if !u.dirty {
		return nil
	}

	c := SharedDBSession().DB(DatabaseName).C("user")
	_, err := c.Upsert(bson.M{"_id": u.udb.UserId}, &u.udb)
	if err != nil {
		CALog.Error("user %s Store err %v\n", u.udb.UserId, err)
		return err
	}

	return nil
}

// I don't knwo how to update part of a doc,
// this func doen't work correctly.
func (u *User) fastStore(id string, change interface{}) error {
	c := SharedDBSession().DB(DatabaseName).C("user")
	_, err := c.Upsert(bson.M{"_id": id}, change)
	if err != nil {
		CALog.Error("fastStore err %v\n", err)
		return err
	}

	return nil
}

func (u *User) Init(c *Client) {
	u.c = c
	u.ready = false
	u.dirty = false
}

func (u *User) Logout() {
	u.ready = false
}

func (u *User) Login(userid string) error {
	if err := u.Load(userid); err != nil {
		u.c.SetErrCode(kErrReadDB)
		return errors.New("DBError")
	}

	CALog.Debug("user %s last login %v\n",
		userid, time.Unix(int64(u.udb.LastLogin), 0))

	// for test
	if u.udb.Heart < 30 {
		u.udb.Heart = 99
	}

	u.udb.LastLogin = uint32(time.Now().Unix())
	u.ready = true
	return nil
}

func (u *User) IsLogin() bool {
	return u.ready
}

func (u *User) AddHeart(heart int) error {
	u.udb.Heart = (uint32)(int(u.udb.Heart) + heart)
	return nil
}

func (u *User) UseHeart(heart int) error {
	if heart < 0 {
		return ErrInvalidParam
	}
	return u.AddHeart(-heart)
}

func GenerateUniqueUserid() string {
	timestamp := time.Now().UnixNano()
	return strconv.FormatInt(timestamp, 10)
}

func ProcUserLogin(c *Client, msg *Msg) int {
	if c.IsLogin() {
		return CLI_PROC_RET_SUCC
	}

	// check if userid is empty
	if msg.Userid == "" {
		msg.Userid = GenerateUniqueUserid();
		CALog.Info("register new user %s\n", msg.Userid)
	}

	// TODO: check if user already login in other server
	if err := c.Login(msg.Userid); err != nil {
		return CLI_PROC_RET_KICK
	}

	c.InsertOnline(msg.Userid)

	// send reply
	reply := c.GetReplyMsg()
	// may changed
	reply.Userid = msg.Userid
	reply.Body["Heart"] = c.udb.Heart
	reply.Body["Stageid"] = c.udb.NextStage
	reply.Body["Panty"] = true
	reply.Body["Tapjoy"] = true

	if len(c.udb.EventLock) == 0 {
		c.udb.EventLock = make([]int, 6, 6)
		for i := 0; i < 6; i++ {
			c.udb.EventLock[i] = 1
		}
	}

	reply.Body["EventLock"] = c.udb.EventLock
	return CLI_PROC_RET_SUCC
}

func ProcIAPAddHeart(c *Client, msg *Msg) int {
	addheart, ok := msg.Body["addheart"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	c.AddHeart(int(addheart.(float64)))
	c.dirty = true

	reply := c.GetReplyMsg()
	reply.Body["Heart"] = c.udb.Heart
	return CLI_PROC_RET_SUCC
}

func ProcStartPlay(c *Client, msg *Msg) int {
	stageid, ok := msg.Body["stageid"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	sid := uint32(stageid.(float64))
	if sid > c.udb.NextStage || sid == 0 {
		c.SetErrCode(kErrInvalidParamters)
		return CLI_PROC_RET_ERR
	}

	if c.udb.Heart < StageHeartConfig[sid - 1] {
		c.SetErrCode(kErrHeartNotEnough)
		return CLI_PROC_RET_ERR
	}

	c.UseHeart(int(StageHeartConfig[sid - 1]))
	c.dirty = true

	// reply
	reply := c.GetReplyMsg()
	reply.Body["Heart"] = c.udb.Heart
	reply.Body["Stageid"] = c.udb.NextStage
	return CLI_PROC_RET_SUCC
}

func ProcEndPlay(c *Client, msg *Msg) int {
	stageid, ok := msg.Body["stageid"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	sid := uint32(stageid.(float64))
	if sid > c.udb.NextStage {
		c.SetErrCode(kErrInvalidParamters)
		return CLI_PROC_RET_ERR
	} else if sid == c.udb.NextStage {
		pass, ok := msg.Body["pass"]
		if !ok {
			c.SetErrCode(kErrInvalidProto)
			return CLI_PROC_RET_ERR
		}
		
		if pass.(bool) {
			c.udb.NextStage++
			c.dirty = true
		}
	}

	// reply
	reply := c.GetReplyMsg()
	reply.Body["Heart"] = c.udb.Heart
	reply.Body["Stageid"] = c.udb.NextStage
	return CLI_PROC_RET_SUCC
}

func ProcEventLock(c *Client, msg *Msg) int {
	stageid, ok := msg.Body["stageid"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	eventid, ok := msg.Body["eventid"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	// unlock
	sid := int(stageid.(float64))
	eid := int(eventid.(float64))
	c.udb.EventLock[sid - 1] = eid

	// use heart
	if sid > 0 && sid <= len(EventHeartConfig) {
		c.UseHeart(int(EventHeartConfig[sid - 1]))
	}

	c.dirty = true

	// reply
	reply := c.GetReplyMsg()
	reply.Body["Heart"] = c.udb.Heart
	reply.Body["Stageid"] = sid
	reply.Body["Eventid"] = eid
	return CLI_PROC_RET_SUCC
}

func ProcUseItem(c *Client, msg *Msg) int {
	itemid, ok := msg.Body["itemid"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	iid := int(itemid.(float64))
	// use heart
	if iid > 0 && iid <= len(ItemHeartConfig) {
		c.UseHeart(int(ItemHeartConfig[iid]))
	}
	c.dirty = true

	// reply
	reply := c.GetReplyMsg()
	reply.Body["Heart"] = c.udb.Heart
	return CLI_PROC_RET_SUCC
}

func AddHeartAsyncOnline(c *Client, count int) {
	c.ichan <- count
}

func AddHeartAsync(userid string, count int) {
	c := CAOnline.Find(userid)
	if c != nil {
		CALog.Debug("%s async add %d", userid, count)
		AddHeartAsyncOnline(c, count)
		return
	}

	CALog.Debug("%s db add %d", userid, count)
	var user User
	user.Init(nil)

	err := user.Load(userid)
	if err != nil {
		CALog.Error("load %s error %v", userid, err)
		return
	}

	user.AddHeart(count)
	user.dirty = true;
	err = user.Store()
	if err != nil {
		CALog.Error("store %s error %v", userid, err)
	}
}
