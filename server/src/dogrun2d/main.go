package main

import "fmt"
import "os"
import "net"
import "dogrun2"

func clientRequest(conn *net.TCPConn) {
	c := dogrun2.NewClient(conn)
	c.Proc()
}

func main() {
	fmt.Printf("Start Server...\n")

	// init db
	dogrun2.ConnectMongo("127.0.0.1")

	// TODO: move to args or config file
	service := ":20000"

	addr, err := net.ResolveTCPAddr("tcp", service)
	checkError(err)

	lis, err := net.ListenTCP("tcp", addr)
	checkError(err)

	for {
		conn, err := lis.AcceptTCP()
		if err != nil {
			fmt.Println(err)
			continue
		}

		go clientRequest(conn)
	}
}

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	}
}
