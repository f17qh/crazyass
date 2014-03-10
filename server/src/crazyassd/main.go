package main

import "fmt"
import "log"
import "code.google.com/p/go.net/websocket"
import "net/http"
import "crazyass"

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
	client := crazyass.NewClient(ws)
	client.Proc()
}

func main() {
	fmt.Printf("Starting crazyassd...\n");
	http.Handle("/echo", websocket.Handler(echo));
	http.Handle("/ca", websocket.Handler(clientProc));
	if err := http.ListenAndServe(":1234", nil); err != nil {
		log.Fatal("ListenAndServe err %v", err)
	}
	fmt.Printf("Stoping crazyassd...\n");
}
