package crazyass

import "errors"
import "log"
import "time"
import "labix.org/v2/mgo"
import "labix.org/v2/mgo/bson"

var ErrInvalidParam = errors.New("Invalid paramters")

func init() {
	// register login cmd
	ClientProcRegister(kCmdUserLogin, ProcUserLogin)
	ClientProcRegister(kCmdStartPlay, ProcStartPlay)
	ClientProcRegister(kCmdEndPlay, ProcEndPlay)
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
}

func userRegister(u *UserDb, id string) {
	u.UserId = id
	u.Heart = 9
	u.NextStage = 1
	u.RegTime = uint32(time.Now().Unix())
	// u.LastLogin = u.RegTime

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
		log.Println(err)
		return err
	}
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
		log.Printf("Store err %v\n", err)
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
		log.Printf("fastStore err %v\n", err)
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

	log.Printf("user %s last login %v\n",
		userid, time.Unix(int64(u.udb.LastLogin), 0))

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

func ProcUserLogin(c *Client, msg *Msg) int {
	if c.u.IsLogin() {
		return CLI_PROC_RET_SUCC
	}

	// TODO: check if user already login in other server

	if err := c.u.Login(msg.Userid); err != nil {
		return CLI_PROC_RET_KICK
	}

	// send reply

	return CLI_PROC_RET_SUCC
}

func ProcStartPlay(c *Client, msg *Msg) int {
	stageid, ok := msg.Body["stageid"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	if uint32(stageid.(float64)) != c.u.udb.NextStage {
		c.SetErrCode(kErrInvalidParamters)
		return CLI_PROC_RET_ERR
	}

	if c.u.udb.Heart < StageHeartConfig[c.u.udb.NextStage] {
		c.SetErrCode(kErrHeartNotEnough)
		return CLI_PROC_RET_ERR
	}

	c.u.UseHeart(int(StageHeartConfig[c.u.udb.NextStage]))
	c.u.dirty = true
	return CLI_PROC_RET_SUCC
}

func ProcEndPlay(c *Client, msg *Msg) int {
	stageid, ok := msg.Body["stageid"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}

	if uint32(stageid.(float64)) != c.u.udb.NextStage {
		c.SetErrCode(kErrInvalidParamters)
		return CLI_PROC_RET_ERR
	}

	pass, ok := msg.Body["pass"]
	if !ok {
		c.SetErrCode(kErrInvalidProto)
		return CLI_PROC_RET_ERR
	}
	
	if pass.(bool) {
		c.u.udb.NextStage++
		c.u.dirty = true
	}
	return CLI_PROC_RET_SUCC
}
