package dogrun2

import _ "testing"
import "net"

func ExampleClientProc() {
	var conn *net.TCPConn
	conn = nil
	c := NewClient(conn)
	c.Proc()
	// Output:
	// >>>
}