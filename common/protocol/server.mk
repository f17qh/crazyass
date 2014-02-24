.PHONY:all go

ifeq ($(OS),Windows_NT)
PROTOC := ../tools/protoc.exe
else
PROTOC := ../../server/bin/protoc
endif

SOURCE := cs.proto userinfo.proto dogattr.proto
TARGET := $(SOURCE:.proto=.pb.cc)
GODIR := ../../server/src/dogrun2cs
GOTARGET := $(SOURCE:.proto=.pb.go)
GOTARGET := $(addprefix $(GODIR)/, $(GOTARGET))

all: $(GOTARGET)

%.pb.cc: %.proto
	$(PROTOC) -I./ --cpp_out=./ $<

$(GODIR)/%.pb.go: %.proto
	mkdir -p $(GODIR)
	$(PROTOC) -I./ --go_out=$(GODIR) $<

clean:
	rm -f *.pb.cc
	rm -f *.pb.h
	rm -f *.go

#../obj/protobuf/bin/protoc -I./ --cpp_out=./ cs.proto
