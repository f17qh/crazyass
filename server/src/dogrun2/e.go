package dogrun2

import "errors"

type SError struct {
	Errno int
	Error error
}

const (
	ERR_READPACKET = 1
	ERR_UNMARSHALPB = 2
	ERR_INVALIDHEADER = 3
	ERR_NR = 4
)

var errArray []SError

func init() {
	errArray = []SError {
		{ERR_READPACKET, errors.New("Read packet from client error")},
		{ERR_UNMARSHALPB, errors.New("Unmarshal protobuff message error")},
		{ERR_INVALIDHEADER, errors.New("Invalid message header")},
	}
}

func ErrGet(errno int) error {
	return errArray[errno].Error
}
