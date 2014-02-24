package main

import "net"
import "io"
import "fmt"

func tcpHandle(conn *net.TCPConn) {
	var buf [10]byte
	_, err := io.ReadFull(conn, buf[0:])
	if err != nil {
		fmt.Println(err)
		return;
	}
	fmt.Println(string(buf[0:]))
	conn.Write(buf[0:])
}

func main() {
	addr, err := net.ResolveTCPAddr("tcp4", ":23456")
	ln, err := net.ListenTCP("tcp", addr)
	if err != nil {
		fmt.Println(err)
		return
	}
	for {
		conn, err := ln.AcceptTCP()
		if err != nil {
			fmt.Println(err)
			return
		}

		go tcpHandle(conn)
	}
}
