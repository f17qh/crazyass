package main

import "fmt"
import "labix.org/v2/mgo"
import "labix.org/v2/mgo/bson"

type Array struct {
	V1 int
	V2 string
}

type Person struct {
	Id string `bson:"_id"`
	Name string
	Phone string
	Array [3]Array
}

func main() {
	session, err := mgo.Dial("127.0.0.1")
	if err != nil {
		panic(err)
	}
	defer session.Close()

	session.SetMode(mgo.Monotonic, true)
	c := session.DB("test").C("people")

	c.DropCollection()

	p := Person{}
	for i := 0; i < 10000; i++ {
		p.Id = fmt.Sprintf("%d", i)
		p.Name = fmt.Sprintf("Ale%d", i)
		p.Phone = fmt.Sprintf("Phone%d", i)
		p.Array[0].V1 = i
		p.Array[0].V2 = p.Name
		p.Array[1].V1 = i + 1
		p.Array[1].V2 = p.Name
		p.Array[2].V1 = i + 2
		p.Array[2].V2 = p.Name
		err = c.Insert(&p)
		if err != nil {
			panic(err)
		}
	}

	result := Person{}
	for i := 0; i < 10000; i++ {
		err = c.Find(bson.M{"_id": fmt.Sprintf("%d", i)}).One(&result)
		if err != nil {
			panic(err)
		}
	}
}