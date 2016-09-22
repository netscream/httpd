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
//#include <arpa/inet.h>
#include <glib.h>
#include <sys/stat.h> //logfile handling
#include <fcntl.h>
//#include <glib/gprintf.h> //for g_strsplit()
#include "debugging.h"
#include "printing.h"

/* concurency specific macros */
#define MAXCLIENTS 1
/* end of concurency spcific macros */

/* HTTP specific macros */
#define HTTP_GET "GET"
#define HTTP_POST "POST"
#define HTTP_HEAD "HEAD"
#define COLOR_PREFIX "/colour?bg="
/* end of HTTP specific macros */
struct REQUESTSTUFF {

} REQUESTSTUFF;
#define LOGFILE "httpd.log"

int runServer(int PortNum);
int sockInit();
struct sockaddr_in serverStructInit(int PortNum);
void bindListenInit(struct sockaddr_in server, int sockfd);
void decodeMessage(int sockfd, struct sockaddr_in *client, int clientLen, char* message);
void logToFile(struct sockaddr_in client, char* request, char* response, char* requestedUrl);
void createHeader(char* header, int sizeOfContent);
void generateHTML(char* buffer, struct sockaddr_in client, int method, char* postBuffer, char* requestPage);
#endif