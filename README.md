# Http server created for computer networks class 

# attn: not complete, other classes consumed our time

Readme for httpd server v0.1 *Still in alpha*

  *Table of contents*

1 => About application
	|- info about this application
2 => Usage
	|- how to use this application
	2.1 => testing
		|- How to test
		|- GET request
		|- POST request
		|- HEADER request
		|- other options
3 => Implement description
	|- Description about how the server is implemented
4 => File tree
	|- File tree and comments about each file
5 => Comments
	|- Other comments about application
6 => Version History
	|- Version history for implemented features


---------------------------------------------------------------
1 => About application
Most of debugging and printing options copied from TFTP assignment.
ghashtable implemented for hashing of attributes from URL
htttp server made persistand.

---------------------------------------------------------------
2 => Usage

./httpd [PORT]

[PORT] - port number that is not in use on this system

For developement:
	$ make                (Will compile the server)
	$ make & make install (Will compile the server and install) (*needs evelated priveledges* **root access in most cases**)
	$ make distclean	  (Will clean all compiled .o and application files)

	2.1 => testing
	For testing use CURL
	PORT = port from starting server

		2.1.1 => GET request
			curl -i http://127.0.0.1:PORT
		2.1.2 => POST request
			curl -i -d "Some new TEXT" http://127.0.0.1:PORT
		2.1.3 => HEAD request
			curl -i -X HEAD http://127.0.0.1:PORT
		2.1.4 => Arguments in URL
			curl -i http://127.0.0.1:PORT/colour?bg=COLOR
				where color = red, blue, green, black, white .... you get the point ;)
		2.1.5 => OTHER
			curl -i -X OTHER http://127.0.0.1:PORT

---------------------------------------------------------------
3 => Implement description

httpd.c: 
	
--------------------------
server.c: (server.h)
	runServer():
		Runs the server in infinit loop and waits for connection to socket.
		when a message is received from socket it returns it to decodeMessage().

	sockInit():
		Initalize the socket

	sockaddr_in serverStructInit()
		Create the Server sockaddr_in struct, memset and initalize

	bindListenInit():
		Bind the server to a socket

	decodeMessage():
		Decode the received message

	logToFile():
		Log everything to logfile, located where you initally start the server

	createHeader():
		Create the http header

	generateHTML():
		Create the HTML in memory

	createUriHashTable():
		Create hash table for the URI query

	deleteAllUriHashTable():
		Delete everything from hashtable

	keyToValueFromHashtable():
		Get value from key outof hashtable
--------------------------
printing.c: (printing.h)
	printToOutput():
		Prints input message to the stdout, put concats a timestamp to the output.

	printToOutputSendHeader():
		Prints to stdout the request header from client

	printToOutputError():
		Prints to the stdout all error messages that are sent to client, that is opcode error.

	printBanner():
		Only prints the beutyful banner on start, *So pretty*

	getHeaderTime():
		Gets the time as a string for header sending.
--------------------------
debugging.c: (debugging.h)  // note this is only for debugging the applictaion.
	// to be able to use these functions please change the "#define debug" macro inside debugging.h to 1 etc. (#define debug 1)

	printTime():
		Function that only prints out the exact server time to the output (note: SERVER TIME)

	debugS():
		For writing strings to the output from other functions, concats the timestamp and "[DEBUG]" to the output so the user knows what info is what info.

	debugD():
		For writing strings with integer values to the stdout, concats timestamp and "[DEBUG]" to the string.

	debugSockAddr():
		for sockaddr and sockaddr_in debugging

	debugMessage():
		for debugging packages sent over the socket.

---------------------------------------------------------------
4 =>File tree for pa1 folder
.
├── AUTHORS		*List of authors*
├── data		*Data directory*
├── pa2.pdf		*PDF file for project, about implementation*
├── README		*THIS FILE!!*
└── src			*src folder*
    ├── libs		*Library folder*
    │   ├── debugging.c	*Debugging c file*
    │   ├── debugging.h *Debugging header file*
    │   ├── printing.c	*Printing c file*
    │   ├── printing.h	*Printing header file*
    │   ├── server.c	*Server c file*
    │   └── server.h	*Server header file*
    ├── Makefile	*Makefile for the server application*
    └── httpd.c		*The main code file*

---------------------------------------------------------------
5 => Comments


---------------------------------------------------------------
6 => Version History
n/a still needed for implementation
