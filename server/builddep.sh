#!/bin/sh

export PATH=`pwd`/bin/:$PATH
make -C ../common/dep/protobuf-2.5.0 -f protobuf.mk
