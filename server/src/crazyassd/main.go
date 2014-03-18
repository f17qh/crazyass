package main

import "log"
import "code.google.com/p/go.net/websocket"
import "net/http"
import "fmt"
import "crazyass"
import "crypto/md5"

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

func HandleTP(w http.ResponseWriter, req *http.Request) {
	req.ParseForm()
	// fmt.Println(req.FormValue)
	fmt.Println(req.Form)

	snuid := req.Form["snuid"][0]
	id := req.Form["id"][0]
	currency := req.Form["currency"][0]
	verifier := req.Form["verifier"][0]

	md5byte := md5.Sum([]byte(id + ":" + snuid + ":" + currency + ":" + "OyOPgaTvwJg3jO6NtnqS"))
	fmt.Printf("md5sum:%s\n", string(md5byte[0:]))
	fmt.Printf("verifier:%s\n", verifier)
	w.Write([]byte(""))
}

func main() {
	log.Printf("Starting crazyassd...");
	crazyass.ConnectMongo("127.0.0.1");
	http.HandleFunc("/tj", HandleTP)
	http.Handle("/echo", websocket.Handler(echo));
	myproto := websocket.Server{
		Handshake: nil,
		Handler: websocket.Handler(clientProc),
	}
	http.Handle("/ca", myproto)
	if err := http.ListenAndServe(":12345", nil); err != nil {
		log.Fatal("ListenAndServe err %v", err)
	}
	log.Printf("Stoping crazyassd...");
}
