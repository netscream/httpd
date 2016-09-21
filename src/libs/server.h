#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h> //??????
#include <glib.h>
#include "debugging.h"
#include "printing.h"

/* concurency specific macros */
#define MAXCLIENTS 1
/* end of concurency spcific macros */

/* HTTP specific macros */
#define HTTP_GET "GET"
#define HTTP_POST "POST"
#define HTTP_HEAD "HEAD"
/* end of HTTP specific macros */
#define LOGFILE "./log/httpd.log"

int runServer(int PortNum);
int sockInit();
struct sockaddr_in serverStructInit(int PortNum);
void bindListenInit(struct sockaddr_in server, int sockfd);

#endif