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
                        message[sizeof(message)-1] = 0;
                        debugS(message);
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