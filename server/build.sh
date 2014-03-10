#!/bin/sh

export GOPATH=`pwd`
export PATH=$GOPATH/bin:$PATH

#go install code.google.com/p/goprotobuf/protoc-gen-go
#go install code.google.com/p/goprotobuf/proto
go install code.google.com/p/go.net/websocket
#make -C ../common/protocol -f server.mk
go install crazyass
go install crazyassd
