CC = gcc
DEBUG = -g3
CPPFLAGS =
CFLAGS = ${DEBUG} -std=c11 -D_XOPEN_SOURCE=700 -O2 -Wall -Wextra -Wformat=2
PROTECTFLAGS = -fstack-protector-all -Wstack-protector -fno-omit-frame-pointer
LDFLAGS =
LOADLIBES =
LDLIBS = 
GLIBUBUNTUFIX = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
    -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
GLIBS = ${GLIBUBUNTUFIX} `pkg-config --cflags --libs glib-2.0`
INSTALLDIR = /usr/local/bin

.DEFAULT: all
.PHONY: all
all: httpdserver

httpdserver: debugging.o printing.o server.o httpd.o
	${CC} ${CFLAGS} ${PROTECTFLAGS} ${GLIBS} debugging.o printing.o server.o httpd.o -lglib-2.0 -lm -o httpd

debugging.o: libs/debugging.c
	${CC} ${CFLAGS} ${GLIBS} libs/debugging.c -c

printing.o: libs/printing.c
	${CC} ${CFLAGS} ${GLIBS} libs/printing.c -c

server.o: libs/server.c
	${CC} ${CFLAGS} ${GLIBS} libs/server.c -c

httpd.o: httpd.c
	${CC} ${CFLAGS} ${GLIBS} httpd.c -c

install: httpd
	install -m 0755 httpd ${INSTALLDIR}

clean:
	rm -f *.o

distclean: clean
	rm -f httpd
