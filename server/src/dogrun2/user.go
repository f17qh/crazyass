package dogrun2

import "errors"
import "log"
import "time"
import "dogrun2cs"
import "labix.org/v2/mgo"
import "labix.org/v2/mgo/bson"
import proto "code.google.com/p/goprotobuf/proto"

var ErrInvalidParam = errors.New("Invalid paramters")

func init() {
	// register login cmd
	ClientProcRegister(int(dogrun2cs.Command_kCmdUserLoginReq), ProcUserLogin)
}

// user in memory
type User struct {
	// whether or not user is login
	ready bool
	// client info
	c *Client
	// data struct saved in db
	udb UserDb
	// dogs in db
	dk DogKeeper
	// whether of not user info is changed.
	// true means should sync to db
	dirty bool
}

type UserDbAttr struct {
	Money uint32
	Heart uint32
}

type UserDb struct {
	UserId string `bson:"_id"`
	Name string
	Title string
	Attr UserDbAttr
	LastLogin uint32
}

func (u *User) Load(userid string) error {
	// load user
	c := SharedDBSession().DB("dogrun2").C("user")
	err := c.Find(bson.M{"_id": userid}).One(&u.udb)
	if err == mgo.ErrNotFound {
		u.udb.UserId = userid
	} else if err != nil {
		log.Println(err)
		return err
	}

	// load dog
	if err = u.dk.Load(userid); err != nil {
		log.Println(err)
		return err
	}

	u.ready = true
	return nil
}

func (u *User) Store() error {
	c := SharedDBSession().DB("dogrun2").C("user")
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
	c := SharedDBSession().DB("dogrun2").C("user")
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
		return errors.New("DBError")
	}

	log.Printf("user %s last login %v\n",
		userid, time.Unix(int64(u.udb.LastLogin), 0))

	// TODO: update login timestamp
	u.udb.LastLogin = uint32(time.Now().Unix())
//	err := u.fastStore(userid, &bson.M{"lastlogin": u.udb.LastLogin})
//	if err != nil {
//		log.Printf("Login err %v\n", err)
//		return err
//	}
	if err := u.Store(); err != nil {
		return errors.New("DBError")
	}

	u.ready = true
	return nil
}

func (u *User) Notify() error {
	pb := dogrun2cs.UserInfo {
		Name: &u.udb.Name,
		Attr: &dogrun2cs.UserAttr {
			Gold: &u.udb.Attr.Money,
			Heart: &u.udb.Attr.Heart,
		},
	}

	msg := u.c.GetReplyMsg()
	msg.H.Cmd = uint32(dogrun2cs.Command_kCmdUserInfoNtf)
	return u.c.SendNotify(msg, &pb);
}

func (u *User) IsLogin() bool {
	return u.ready
}

func (u *User) AddMoney(money int) error {
	u.udb.Attr.Money = (uint32)(int(u.udb.Attr.Money) + money)
	return nil
}

func (u *User) UseMoney(money int) error {
	if money < 0 {
		return ErrInvalidParam
	}
	return u.AddMoney(-money)
}

func (u *User) AddHeart(heart int) error {
	u.udb.Attr.Heart = (uint32)(int(u.udb.Attr.Heart) + heart)
	return nil
}

func (u *User) UseHeart(heart int) error {
	if heart < 0 {
		return ErrInvalidParam
	}
	return u.AddMoney(-heart)
}

func ProcUserLogin(c *Client, msg *Msg) int {
	if c.u.IsLogin() {
		return CLI_PROC_RET_SUCC
	}

	// TODO: check if user already login in other server

	var req dogrun2cs.UserLoginReq
	if err := proto.Unmarshal(msg.Body, &req); err != nil {
		return CLI_PROC_RET_KICK
	}

	if err := c.u.Login(req.GetUserid()); err != nil {
		return CLI_PROC_RET_KICK
	}

	return CLI_PROC_RET_SUCC
}
