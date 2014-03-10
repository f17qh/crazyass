package crazyass

import "fmt"
import "os"
import "time"
import "errors"
import "log"
import "code.google.com/p/go.net/websocket"
import _ "encoding/json"

type Msg struct {
	Cmd float64
	ErrCode float64
	Userid string
	Body map[string]interface{}
}

// each client has a Client object
// we can pre-alloc many Client objects for performance
type Client struct {
	// save current user data
	u User

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
}

const (
	CLI_PROC_RET_SUCC = iota
	CLI_PROC_RET_ERR  = iota
	CLI_PROC_RET_KICK = iota
)

type ClientProc func(*Client, *Msg) int

// cmd proc func array
var procFuncArray [128]ClientProc

func init() {
	// init log
	log.SetFlags(log.Flags() | log.Lshortfile)

	for i := 0; i < 128; i++ {
		procFuncArray[i] = nil
	}

	// cmd 0 is reserved
	t := func(c *Client, msg *Msg) int {
		return 0
	}

	ClientProcRegister(0, t)
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
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	}
}

func (c *Client) SetErrCode(errno int) {
	// c.errcode = errno
	c.replyMsg.ErrCode = float64(errno)
}

// main process function for each client
func (c *Client) Proc() {
	// clear Client tmp data
	defer c.Close()
	req_chan := make(chan *Msg, 1)
	// read request from client
	// process req and send rsp
	for c.enable {
		// clear last error
		c.lastErr = ""
		c.SetErrCode(0)

		select {
		case msg := <-c.readMsg(req_chan):
			if msg != nil {
				c.procMsg(msg)
			}
		case <-time.After(2 * time.Second):
			c.procTimeout()
		}
	}

	// end here
}

// read packet from websocket conn
func (c *Client) readMsg(req_chan chan *Msg) chan *Msg {
	if msg, err := c.readPacket(c.conn); err != nil {
		// if readPacket err, close client
		c.enable = false
		req_chan <- nil
	} else {
		req_chan <- msg
	}
	return req_chan
}

// proc request by cmd
func (c *Client) procMsg(msg *Msg) {
	if msg.Cmd >= 128 {
		log.Printf("Unknown cmd\n")
		return
	}

	// init reply msg
	c.replyMsg.Userid = msg.Userid
	c.replyMsg.Cmd = msg.Cmd

	// proc msg
	ret := CLI_PROC_RET_SUCC
	if !c.u.IsLogin() && msg.Cmd != kCmdUserLogin {
		ret = CLI_PROC_RET_KICK
	} else {
		proc := procFuncArray[int(msg.Cmd)]
		ret = proc(c, msg)
	}

	// flush data to db
	if c.u.Store() != nil {
		// database error, kick off user
		ret = CLI_PROC_RET_KICK
		c.SetErrCode(kErrWriteDB)
	}

	// proc error and client should be kick off
	if ret == CLI_PROC_RET_KICK {
		c.enable = false
	}

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
	c.u.Init(c)
	return c
}

func (c *Client) readPacket(conn *websocket.Conn) (*Msg, error) {
	var err error
	if err = websocket.JSON.Receive(conn, &c.msg); err != nil {
		log.Fatal("Receive err %v\n", err)
		return nil, err
	}

	return &c.msg, nil
}
