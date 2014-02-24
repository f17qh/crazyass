package main

import "net/http"
import "log"
import "fmt"
import "flag"

var port = flag.Int("port", 80, "default port")
var rootdir = flag.String("rootdir", "./", "default port")

func foodhandle(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello");
}

func main() {
	flag.Parse()
	// serve static under an alternate URL
	http.Handle("/", http.StripPrefix("/", http.FileServer(http.Dir(*rootdir))))
	http.HandleFunc("/foo", foodhandle)
	err := http.ListenAndServe(fmt.Sprintf(":%d", *port), nil)
	if err != nil {
		log.Fatal("fatal %p", err)
	}
}
