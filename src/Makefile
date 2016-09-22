CC = gcc
CPPFLAGS =
CFLAGS = -std=c11 -D_XOPEN_SOURCE=700 -O2 -Wall -Wextra -Wformat=2
LDFLAGS =
LOADLIBES =
LDLIBS = 
GLIBS = `pkg-config --cflags --libs glib-2.0`
INSTALLDIR = /usr/local/bin

.DEFAULT: all
.PHONY: all
all: httpdserver

httpdserver: debugging.o printing.o server.o httpd.o
	${CC} ${CFLAGS} ${GLIBS} debugging.o printing.o server.o httpd.o -o httpd

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
