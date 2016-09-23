#include "server.h"



int runServer(int PortNum)
{
	int sockfd = -1;
    int retSel = -1;
    struct sockaddr_in server, client;
    char message[512];
    fd_set readSocketFd;
    sockfd = sockInit(); //initalize socket
    server = serverStructInit(PortNum);
    bindListenInit(server, sockfd);

    FD_ZERO(&readSocketFd);
    FD_SET(sockfd, &readSocketFd);
    while(1)
    {   
        memset(&message, 0, sizeof(message));
        memset(&client, 0, sizeof(client));
        retSel = -1;
        int newConnectionFd = -1;
        socklen_t clientLen = (socklen_t) sizeof(client);
        debugS("Before select()");
        if ((retSel = select(sockfd+MAXCLIENTS, &readSocketFd, 0, 0, 0)) == -1)
        {
            perror("Select() error: ");
        }
        else 
        if (retSel > 0)
        {
            debugS("Retsel > 0");
            for (int nextSock = 0; nextSock < sockfd+MAXCLIENTS; nextSock++) //FD_SETSIZE = 1024 man select()
            {
                if (FD_ISSET(nextSock, &readSocketFd))
                {
                    if (nextSock == sockfd)
                    {
                        debugS("Trying to accept new connection");
                        newConnectionFd = accept(sockfd, (struct sockaddr*) &client, &clientLen);
                        if (newConnectionFd == -1)
                        {
                            perror("Accept error: ");
                        }
                        int sizeofMes = read(newConnectionFd, message, sizeof(message)-1);
                        if (sizeofMes == -1)
                        {
                            perror("Read from socket error: ");
                        }
                        message[sizeof(message)-1] = 0;
                        debugSockAddr(message, client);
                        decodeMessage(newConnectionFd, &client, clientLen, message);
                        shutdown(newConnectionFd, SHUT_RDWR);
                        close(newConnectionFd);
                    }
                }
            }
        }
        else 
        if (retSel == 0)
        {
            debugS("No filedescriptors available");
        }
        debugS("After retsel");
	}
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
	return -1;
}

/*
 * Function sockInit()
 * Creates socket and sets options for the socket.
 */
int sockInit()
{
    /*  
     *  From man page
     *  SOCK_STREAM
     *  Provides sequenced, reliable, two-way, connection-based byte streams. An out-of-band data transmission mechanism may be supported. 
     */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sockfd == -1) {
        perror("Unable to create socket: ");
        return -1; // no use to continue if no socket, return -1 with error.
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 0, sizeof(SO_REUSEADDR));  //set reuse of address
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT , 0, sizeof(SO_REUSEPORT ));  //set reuse of port
    return sockfd;
}
/*
 * Function serverStructInit()
 * returns a struct for the server initalization
 */
struct sockaddr_in serverStructInit(int PortNum)
{
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PortNum);
    return server;
}

/*
 * Function bindListenInit
 * void function to bind socket and start listen on port.
 */
void bindListenInit(struct sockaddr_in server, int sockfd)
{
    int retErr = bind(sockfd, (struct sockaddr *) &server, (socklen_t) sizeof(server));
    if (retErr == -1)
    {
        perror("Bind error:");
        exit(1); // no use to continue if unable to bind socket
    }

    retErr = listen(sockfd, MAXCLIENTS);
    if (retErr == -1)
    {
        perror("Listen error:");
        exit(1);
    }
}

void decodeMessage(int sockfd, struct sockaddr_in *client, int clientLen, char* message)
{
    gchar** splitMessage = g_strsplit(message, " ", -1); //-1 = null terminate
    
    debugGMessage(splitMessage, g_strv_length(splitMessage));
    char* response = "HTTP/1.1 200 OK";
    char* request = splitMessage[0];
    char* requestedURL = splitMessage[1];
    if (g_str_has_prefix(splitMessage[0], (gchar*) HTTP_GET))
    {
        debugS("GET");
        char bufferHTML[2048];
        char bufferHEAD[512];
        memset(&bufferHTML, 0, 2048 );
        memset(&bufferHEAD, 0, 512);
        generateHTML(bufferHTML, *client, 0, NULL, requestedURL);
        createHeader(bufferHEAD, sizeof(bufferHTML));
        write(sockfd, &bufferHEAD, sizeof(bufferHEAD));
        write(sockfd, &bufferHTML, sizeof(bufferHTML));
    }
    else
    if (g_str_has_prefix(splitMessage[0], (gchar*) HTTP_POST))
    {
        debugS("POST");
        gchar** splitPostMessage = g_strsplit((gchar*) message, (gchar*) "\n", -1);
        gchar* postContent = splitPostMessage[g_strv_length(splitPostMessage)-1];
        //printf("THIS IS THE CONTENT %s\n", postContent);
        char bufferHTML[2048];
        char bufferHEAD[512];
        memset(&bufferHTML, 0, 2048 );
        memset(&bufferHEAD, 0, 512);
        generateHTML(bufferHTML, *client, 1, postContent, requestedURL);
        createHeader(bufferHEAD, sizeof(bufferHTML));
        write(sockfd, &bufferHEAD, sizeof(bufferHEAD));
        write(sockfd, &bufferHTML, sizeof(bufferHTML));
        g_strfreev(splitPostMessage);
    }
    else    
    if (g_str_has_prefix(splitMessage[0], (gchar*) HTTP_HEAD))
    {
        debugS("HEAD");
        char header[512];
        memset(&header, 0, 512);
        createHeader(header, 0);
        int wrError = -1; 
        wrError = write(sockfd, &header, 512);
        if ( wrError == -1)
        {
            perror("Write error: ");
        }
    }
    logToFile(*client, request, response, requestedURL);
    g_strfreev(splitMessage);
}

void logToFile(struct sockaddr_in client, char* request, char* response, char* requestedUrl)
{
    int len = 20;
    char clBugg[len];
    char buffer[512];
    memset(&buffer, 0, 512);
    char theTime[21];
    char portID[2];
    sprintf(portID,"%d", ntohs(client.sin_port));
    getHeaderTime(theTime, 2);
    FILE *fp;
    fp = fopen(LOGFILE, "a+");
    if (fp == NULL)
    {
        perror("Open logfile error: ");
    }
    strcat(buffer, theTime); //time ISO-8601 compliant
    strcat(buffer, " : ");
    strcat(buffer, inet_ntop(AF_INET, &(client.sin_addr), clBugg, len)); //ip address
    strcat(buffer, ":");
    strcat(buffer, portID); //port
    strcat(buffer, " ");
    strcat(buffer, request);
    strcat(buffer, " ");
    strcat(buffer, requestedUrl);
    strcat(buffer, " : ");
    strcat(buffer, response);
    strcat(buffer, "\n");
    fprintf(fp, "%s", buffer);  
    fclose(fp);
    fp = NULL;
    return;
}

void createHeader(char* header, int sizeOfContent)
{
    char theTime[40];
    getHeaderTime(theTime, 1);
    char theSizeOfContent[4];
    sprintf(theSizeOfContent, "%d", sizeOfContent);
    strcat(header, "HTTP/1.1 200 OK\r\n");
    strcat(header, "Date: ");
    strcat(header, theTime);
    strcat(header, "\r\n");
    strcat(header, "Content-type: text/html\r\n");
    strcat(header, "Content-length: ");
    strcat(header, theSizeOfContent);
    strcat(header, "\r\n");
    strcat(header, "Connection: static\r\n");
    strcat(header, "Server: Foo/1.0\r\n");
    strcat(header, "\r\n\r\n");
    return;
}

void generateHTML(char* buffer, struct sockaddr_in client, int method, char* postBuffer, char* requestPage)
{

    int len = 20;
    char clBugg[len];
    char portID[2];
    sprintf(portID,"%d", ntohs(client.sin_port));
    strcat(buffer, "<!DOCTYPE html>\n");
    strcat(buffer, "<html>\n");
    strcat(buffer, "<title>");
    strcat(buffer, "http://foo.com");
    strcat(buffer, requestPage);
    strcat(buffer, " ");
    strcat(buffer, inet_ntop(AF_INET, &(client.sin_addr), clBugg, len));
    strcat(buffer, ":");
    strcat(buffer, portID);
    strcat(buffer, " ");
    strcat(buffer, "</title>\n");
    strcat(buffer, "<head>\n");
    strcat(buffer, "<meta charset=\"UTF-8\">\n"); 
    strcat(buffer, "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=ISO-8859-1\">\n");
    strcat(buffer, "</head>\n");
    if (g_str_has_prefix(requestPage, (gchar*) COLOR_PREFIX))
    {
        gchar** getColor = g_strsplit(requestPage, "=", -1); //-1 = null terminate
        strcat(buffer, "<body style=\"background-color:");
        strcat(buffer, getColor[1]);
        strcat(buffer, "\">");
        g_strfreev(getColor);
        
    }
    else
    {
        strcat(buffer, "<body>\n");
        if (method == 0)
        {
            strcat(buffer, "<h1> This is a test page </h1>\n");
        }
        else
        {
            strcat(buffer, "<h1>");
            if (postBuffer != NULL)
            {
                strcat(buffer, postBuffer);
            }
            else
            {
                strcat(buffer, "No data posted, please try again!");
            }
            strcat(buffer, "</h1>\n");   
        }
    }
    strcat(buffer, "</body>\n");
    strcat(buffer, "</html>\n");
    return;
}