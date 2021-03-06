package main

import "log"
import "code.google.com/p/go.net/websocket"
import "net/http"
import "fmt"
import "crazyass"
import "crypto/md5"
import "encoding/hex"
import "strconv"
import "code.google.com/p/log4go"
import "time"

var mainlog log4go.Logger
func init() {
	mainlog = make(log4go.Logger)
	// log.AddFilter("stdout", log4go.DEBUG, log4go.NewConsoleLogWriter())
	mainlog.AddFilter("log", log4go.DEBUG, log4go.NewFileLogWriter("../log/main.log", true).SetRotateDaily(true))
	mainlog.Info("The time is now: %s", time.Now().Format("15:04:05 MST 2006/01/02"))
}

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
	mainlog.Info("%v", req.Form)

	snuid := req.Form["snuid"][0]
	id := req.Form["id"][0]
	currency := req.Form["currency"][0]
	verifier := req.Form["verifier"][0]

	md5byte := md5.Sum([]byte(id + ":" + snuid + ":" + currency + ":" + "OyOPgaTvwJg3jO6NtnqS"))
	// fmt.Printf("md5sum:%s\n", hex.EncodeToString(md5byte[0:]))
	// fmt.Printf("verifier:%s\n", verifier)
	if hex.EncodeToString(md5byte[0:]) == verifier {
		c, err := strconv.Atoi(currency)
		if err == nil {
			crazyass.AddHeartAsync(snuid, c)
		}
	}
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
