all:
	mkdir -p ../../obj/protobuf
	if [ ! -e Makefile ]; then \
	  sh ./configure --prefix=`pwd`/../../../server; \
	fi
	make; make install
