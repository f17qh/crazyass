package crazyass

import "os"
import "time"
import "errors"
import "code.google.com/p/go.net/websocket"
import "code.google.com/p/log4go"
import _ "encoding/json"
import "sync"
import _ "sync/atomic"

type Msg struct {
	Cmd float64
	ErrCode float64
	Seq float64
	Userid string
	Body map[string]interface{}
}

// each client has a Client object
// we can pre-alloc many Client objects for performance
type Client struct {
	// save current user data
	// u User
	User

	// current tcp connection
	conn *websocket.Conn

	// this client should be close
	enable bool

	// save last error message
	// if proc() returns ERR or KICK
	// this string will send to client
	lastErr string

	// save client last request
	msg Msg

	// for reply
	replyMsg Msg

	// error code for client
	errcode int

	// internal msg chan
	ichan chan int
}

type Online struct {
	usermap map[string]*Client
	mutex sync.Mutex
}

func (on *Online) Insert(userid string, c *Client) error {
	on.mutex.Lock()
	defer on.mutex.Unlock()

	_, ok := on.usermap[userid]
	if ok {
		return errors.New("User already inserted")
	}

	on.usermap[userid] = c
	CALog.Info("%s login", userid)
	return nil
}

func (on *Online) Delete(userid string) {
	on.mutex.Lock()
	defer on.mutex.Unlock()

	delete(on.usermap, userid)
	CALog.Info("%s logout", userid)
}

func (on *Online) Find(userid string) *Client {
	on.mutex.Lock()
	defer on.mutex.Unlock()

	c, ok := on.usermap[userid]
	if ok {
		return c
	}
	return nil
}

const (
	CLI_PROC_RET_SUCC = iota
	CLI_PROC_RET_ERR  = iota
	CLI_PROC_RET_KICK = iota
)

type ClientProc func(*Client, *Msg) int

// cmd proc func array
var procFuncArray [128]ClientProc
var CALog log4go.Logger
var CAOnline Online

func init() {
	CALog = make(log4go.Logger)
	CALog.AddFilter("log", log4go.DEBUG, log4go.NewFileLogWriter("../log/crazyass.log", true).SetRotateDaily(true))
	CALog.Info("The time is now: %s", time.Now().Format("15:04:05 MST 2006/01/02"))

	for i := 0; i < 128; i++ {
		procFuncArray[i] = nil
	}

	// cmd 0 is reserved
	t := func(c *Client, msg *Msg) int {
		return 0
	}

	ClientProcRegister(0, t)
	CAOnline.usermap = make(map[string]*Client)
}

// Register your client cmd proc function
func ClientProcRegister(cmd int, proc ClientProc) error {
	if cmd >= 128 {
		return errors.New("Command is too big")
	}

	if procFuncArray[cmd] != nil {
		return errors.New("Command is already registed")
	}

	procFuncArray[cmd] = proc
	return nil
}

func checkError(err error) {
	if err != nil {
		CALog.Error("Fatal error: %s", err.Error())
		os.Exit(1)
	}
}

func (c *Client) SetErrCode(errno int) {
	// c.errcode = errno
	c.replyMsg.ErrCode = float64(errno)
}

func (c *Client) InsertOnline(userid string) error {
	return CAOnline.Insert(userid, c)
}

// main process function for each client
func (c *Client) Proc() {
	// clear Client tmp data
	defer c.Close()
	req_chan := make(chan *Msg, 1)

	go c.readMsg(req_chan)

	// read request from client
	// process req and send rsp
	for c.enable {
		// clear last error
		c.lastErr = ""
		c.SetErrCode(0)

		select {
		case msg := <- req_chan:
			if msg != nil {
				c.procMsg(msg)
			}
		case imsg := <-c.ichan:
			c.procIMsg(imsg)
		case <-time.After(300 * time.Second):
			c.procTimeout()
		}
	}

	CAOnline.Delete(c.udb.UserId)
	// end here
}

func (c *Client) procIMsg(value int) {
	c.AddHeart(value)
}

// read packet from websocket conn
func (c *Client) readMsg(req_chan chan *Msg) {
	for {
		if msg, err := c.readPacket(c.conn); err != nil {
			// if readPacket err, close client
			c.enable = false
			req_chan <- nil
			return
		} else {
			req_chan <- msg
		}
	}
}

// proc request by cmd
func (c *Client) procMsg(msg *Msg) {
	if msg.Cmd >= 128 {
		CALog.Error("Unknown cmd\n")
		return
	}

	CALog.Debug("Proc msg %v\n", *msg)

	// init reply msg
	c.replyMsg.Userid = msg.Userid
	c.replyMsg.Cmd = msg.Cmd
	c.replyMsg.Seq = msg.Seq
	// alloc a new map
	// old will be GC eventually
	c.replyMsg.Body = make(map[string]interface{})

	// proc msg
	ret := CLI_PROC_RET_SUCC
	if !c.IsLogin() && msg.Cmd != kCmdUserLogin {
		ret = CLI_PROC_RET_KICK
	} else {
		proc := procFuncArray[int(msg.Cmd)]
		if proc == nil {
			ret = CLI_PROC_RET_KICK
		} else {
			ret = proc(c, msg)
		}
	}

	// flush data to db
	if c.Store() != nil {
		// database error, kick off user
		ret = CLI_PROC_RET_KICK
		c.SetErrCode(kErrWriteDB)
	}

	// proc error and client should be kick off
	if ret == CLI_PROC_RET_KICK {
		c.enable = false
	}

	CALog.Debug("Reply msg %v\n", c.replyMsg)
	// send reply
	websocket.JSON.Send(c.conn, c.replyMsg)
}

// heartbeat timeout
func (c *Client) procTimeout() {
	c.enable = false
}

// client current client
func (c *Client) Close() {
	if c.conn != nil {
		c.conn.Close()
	}
}

func (c *Client) GetReplyMsg() *Msg {
	return &c.replyMsg
}

func NewClient(conn *websocket.Conn) *Client {
	c := &Client{
		conn: conn,
		enable: true,
	}
	c.ichan = make(chan int, 4)
	c.Init(c)
	return c
}

func (c *Client) readPacket(conn *websocket.Conn) (*Msg, error) {
	var err error
	if err = websocket.JSON.Receive(conn, &c.msg); err != nil {
		CALog.Error("Receive err %s\n", err.Error())
		return nil, err
	}

	return &c.msg, nil
}
