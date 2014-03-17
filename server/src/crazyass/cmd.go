package crazyass

// const var for crazyass
import "errors"

// cmd
const (
	kCmdUserLogin = 1
	kCmdStartPlay = 2
	kCmdEndPlay = 3
	kCmdIAPAddHeart = 4
	kCmdEventLock = 5
	kCmdUseItem = 6
)

// error
var ErrReadPacket = errors.New("Read packet from client error")
var ErrUnmarshalMessage = errors.New("Unmarshal protobuff message error")
var ErrInvalidHeader = errors.New("Invalid message header")

// error code
const (
	kErrInvalidParamters = 1
	kErrReadDB = 2
	kErrWriteDB = 3
	kErrHeartNotEnough = 4
	kErrInvalidProto = 5
)

// config
const (
	DatabaseName = "crazyass"
	CrazyAssProtoMagic = 0x20140309
)

var StageHeartConfig = []uint32{2,3,4,5,6,7}
var EventHeartConfig = []uint32{12,12,12,12}

const (
	EventHeartAll = 24
)

// itemid
const (
	ItemPlayAgain1 = 1
	ItemPlayAgain2 = 2
	ItemPlayAgain3 = 3
	ItemPlayAgain4 = 4
	ItemPlayAgain5 = 5
	ItemPlayAgain6 = 6
	ItemGodFinger  = 7
)

var ItemHeartConfig = []uint32{0,3,4,5,6,7,8,4}
