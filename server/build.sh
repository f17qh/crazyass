#!/bin/sh

export GOPATH=`pwd`
export PATH=$GOPATH/bin:$PATH

go install code.google.com/p/goprotobuf/protoc-gen-go
go install code.google.com/p/goprotobuf/proto
make -C ../common/protocol -f server.mk
go install test
go install dogrun2d
go install client