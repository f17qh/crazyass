package main

import "fmt"
import "net"
import "dogrun2cs"
import "dogrun2"
import proto "code.google.com/p/goprotobuf/proto"

func checkError(err error) {
	if err != nil {
		panic("")
	}
}

func main() {
	userid := "lijie"

	// connect
	tcpaddr, err := net.ResolveTCPAddr("tcp4", "127.0.0.1:20000")
	checkError(err)

	conn, err := net.DialTCP("tcp", nil, tcpaddr)
	checkError(err)

	// login
	var req dogrun2cs.UserLoginReq
	req.Userid = proto.String(userid)

	// marshal header
	var data [dogrun2.HeaderSize]byte
	var h dogrun2.Header
	h.Magic = dogrun2.Dogrun2ProtoMagic
	h.Cmd = uint32(dogrun2cs.Command_kCmdUserLoginReq)
	h.Seq = 0
	h.Err = 0
	h.Size = uint32(proto.Size(&req) + dogrun2.HeaderSize)
	h.Marshal(data[0:dogrun2.HeaderSize])

	// marshal body
	buffer := proto.NewBuffer(data[0:])
	err = buffer.Marshal(&req)
	fmt.Println(buffer.Bytes())
	fmt.Println(proto.Marshal(&req))
	checkError(err)

	// Send
	n, err := conn.Write(buffer.Bytes())
	fmt.Printf("send %d bytes\n", n)
	checkError(err)
}
