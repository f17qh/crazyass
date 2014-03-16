package main

import "log"
import "code.google.com/p/go.net/websocket"
import "net/http"
import "crazyass"
import "os"
import "fmt"

func echo(ws *websocket.Conn) {
	var err error
	for {
		var reply string

		if err = websocket.Message.Receive(ws, &reply); err != nil {
			log.Fatal("Receive err %v\n", err)
			break
		}
		if err = websocket.Message.Send(ws, &reply); err != nil {
			log.Fatal("Send err %v\n", err)
			break
		}
		log.Printf("Send and recv %s\n", reply);
	}
}

func clientProc(ws *websocket.Conn) {
	log.Printf("recv client %v\n", *ws);
	client := crazyass.NewClient(ws)
	client.Proc()
}

func initLog() {
	log.SetFlags(log.Flags() | log.Lshortfile)
	f, err := os.OpenFile("crazyass.log", os.O_RDWR | os.O_CREATE | os.O_APPEND, 0666)
	if err != nil {
		fmt.Printf("error opening file: %v", err)
	}

	log.SetOutput(f)
}

func main() {
	initLog()
	log.Printf("Starting crazyassd...\n");
	crazyass.ConnectMongo("127.0.0.1");
	http.Handle("/echo", websocket.Handler(echo));
	myproto := websocket.Server{
		Handshake: nil,
		Handler: websocket.Handler(clientProc),
	}
	http.Handle("/ca", myproto)
	if err := http.ListenAndServe(":12345", nil); err != nil {
		log.Fatal("ListenAndServe err %v", err)
	}
	log.Printf("Stoping crazyassd...\n");
}
