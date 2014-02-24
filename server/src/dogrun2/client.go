package dogrun2

import "net"
import "fmt"
import "os"
import "time"
import "errors"
import "io"
import "log"
import proto "code.google.com/p/goprotobuf/proto"
import "dogrun2cs"

var ErrReadPacket = errors.New("Read packet from client error")
var ErrUnmarshalMessage = errors.New("Unmarshal protobuff message error")
var ErrInvalidHeader = errors.New("Invalid message header")

const (
	Dogrun2ProtoMagic = 0x20140114
)

// each client has a Client object
// we can pre-alloc many Client objects for performance
type Client struct {
	// save current user data
	u User

	// current tcp connection
	conn *net.TCPConn

	// this client should be close
	enable bool

	// save last error message
	// if proc() returns ERR or KICK
	// this string will send to client
	lastErr string

	// save client last request
	msg Msg

	// protobuf Buffer, for marshal reply msg
	pbuf proto.Buffer

	// for reply
	replyMsg Msg
}

// Header for client request packet
type Header struct {
	Magic uint32
	Cmd uint32
	Seq uint32
	Err uint32
	Size uint32
	Userid string
}

// Save client request/response's header and body
type Msg struct {
	H Header
	// protbuf message data
	Body []byte
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

// main process function for each client
func (c *Client) Proc() {
	// clear Client tmp data
	defer c.Close()
	req_chan := make(chan *Msg, 1)
	// read request from client
	// process req and send rsp
	for c.enable {
		// clear last error message
		c.lastErr = ""

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

// read packet from tcp conn
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
	if msg.H.Cmd >= 128 {
		log.Printf("Unknown cmd\n")
		return
	}

	proc := procFuncArray[msg.H.Cmd]
	ret := proc(c, msg)

	// proc error and client should be kick off
	if ret == CLI_PROC_RET_KICK {
		c.enable = false
	}

	// send last error message
	if ret == CLI_PROC_RET_KICK || ret == CLI_PROC_RET_ERR {
	}
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

func (c *Client) Send(buf []byte) (int, error) {
	return c.conn.Write(buf)
}

func (c *Client) sendLastErr() error {
	var s dogrun2cs.SvrErrNtf

	s.Message = &c.lastErr
	msg := c.GetReplyMsg()
	msg.H.Cmd = uint32(dogrun2cs.Command_kCmdSvrErrNtf)
	return c.SendMsg(msg, &s)
}

func (c *Client) SendMsg(msg *Msg, pb proto.Message) error {
	// marshal header
	// TODO: pre-alloc []byte is better
	var data [HeaderSize]byte
	msg.H.Magic = Dogrun2ProtoMagic
	msg.H.Seq = c.msg.H.Seq
	msg.H.Size = uint32(proto.Size(pb) + HeaderSize)
	msg.H.Marshal(data[0:HeaderSize])

	// marshal body
	c.pbuf.Reset()
	err := c.pbuf.Marshal(pb)
	if err != nil {
		log.Printf("Marshal pb error %v\n", err)
		return err
	}

	_, err = c.conn.Write(c.pbuf.Bytes())
	return err
}

func (c *Client) SendNotify(msg *Msg, pb proto.Message) error {
	// marshal header
	// TODO: pre-alloc []byte is better
	var data [HeaderSize]byte
	msg.H.Magic = Dogrun2ProtoMagic
	msg.H.Seq = 0
	msg.H.Size = uint32(proto.Size(pb) + HeaderSize)
	msg.H.Marshal(data[0:HeaderSize])

	// marshal body
	c.pbuf.Reset()
	err := c.pbuf.Marshal(pb)
	if err != nil {
		log.Printf("Marshal pb error %v\n", err)
		return err
	}

	_, err = c.conn.Write(c.pbuf.Bytes())
	return err
}

func (c *Client) GetReplyMsg() *Msg {
	c.replyMsg.H.Magic = uint32(Dogrun2ProtoMagic)
	c.replyMsg.H.Cmd = uint32(0)
	c.replyMsg.H.Seq = uint32(0)
	c.replyMsg.H.Size = uint32(0)
	c.replyMsg.H.Userid = ""
	return &c.replyMsg
}

func NewClient(conn *net.TCPConn) *Client {
	return &Client{
		conn: conn,
		enable: true,
	}
}

const (
	HeaderSize = 20 + 64
)

func parseUint32(buf []byte) uint32 {
	return uint32(buf[0]) << 24 | uint32(buf[1]) << 16 | uint32(buf[2]) << 8 | uint32(buf[3])
}

func parseHeader(buf []byte, h *Header) error {
	h.Magic = parseUint32(buf[0:4])
	if h.Magic != Dogrun2ProtoMagic {
		log.Printf("magic err %x %x\n", h.Magic, Dogrun2ProtoMagic)
		return ErrInvalidHeader
	}
	h.Cmd = parseUint32(buf[4:8])
	if h.Cmd >= 128 {
		return ErrInvalidHeader
	}
	h.Seq = parseUint32(buf[8:12])
	h.Err = parseUint32(buf[12:16])
	h.Size = parseUint32(buf[16:20])
	h.Userid = string(buf[20:])
	return nil
}

func marshlUint32(p uint32, buf []byte) {
	buf[3] = uint8(p)
	buf[2] = uint8(p >> 8)
	buf[1] = uint8(p >> 16)
	buf[0] = uint8(p >> 24)
}

func (h *Header) Marshal(buf []byte) error {
	marshlUint32(h.Magic, buf[0:4])
	marshlUint32(h.Cmd, buf[4:8])
	marshlUint32(h.Seq, buf[8:12])
	marshlUint32(h.Err, buf[12:16])
	marshlUint32(h.Size, buf[16:20])
	copy(buf[20:], h.Userid)
	return nil
}

func (c *Client) readPacket(conn *net.TCPConn) (*Msg, error) {
	var hbuf [HeaderSize]byte

	n, err := io.ReadFull(conn, hbuf[0:])
	if n != HeaderSize || err != nil {
		log.Printf("invalid header %d %v\n", n, err)
		return nil, ErrReadPacket
	}

	if err = parseHeader(hbuf[0:], &c.msg.H); err != nil {
		log.Printf("parse header error\n")
		return nil, err
	}

	bodysize := int(c.msg.H.Size - HeaderSize)
	body := make([]byte, bodysize)
	n, err = io.ReadFull(conn, body)
	if n != bodysize || err != nil {
		return nil, ErrReadPacket
	}

	c.msg.Body = body
	return &c.msg, nil
}
