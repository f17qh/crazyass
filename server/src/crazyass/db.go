package crazyass

import "sync"
import "labix.org/v2/mgo"
import _ "labix.org/v2/mgo/bson"

var mongodbSession *mgo.Session
var dbInitLock sync.Mutex

func init() {
}

func ConnectMongo(ip string) error {
	if mongodbSession != nil {
		return nil
	}

	dbInitLock.Lock()
	defer dbInitLock.Unlock()

	if mongodbSession != nil {
		return nil
	}

	// TODO: read ip from config file
	session, err := mgo.Dial("127.0.0.1")
	if err != nil {
		return err
	}
	session.SetMode(mgo.Monotonic, true)
	mongodbSession = session
	return nil
}

func SharedDBSession() *mgo.Session {
	return mongodbSession
}
