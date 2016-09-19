#include "server.h"


int runServer(int PortNum)
{
	int sockfd = -1;
	int retErr = -1;
    struct sockaddr_in server, client;
    fd_set readSocketFd;

    /*  
     *  From man page
     *  SOCK_STREAM
     *  Provides sequenced, reliable, two-way, connection-based byte streams. An out-of-band data transmission mechanism may be supported. 
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sockfd == -1) {
        perror("Unable to create socket: ");
        return -1; // no use to continue if no socket, return -1 with error.
    }

    /* Lets zero config the server sockaddr struct */
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PortNum);

    retErr = bind(sockfd, (struct sockaddr *) &server, (socklen_t) sizeof(server));
    if (retErr == -1)
    {
        perror("Bind error:");
        return -1; // no use to continue if unable to bind socket
    }

    retErr = listen(sockfd, MAXCLIENTS);
    if (retErr == -1)
    {
    	perror("Listen error:");
    	return -1;
    }

    FD_ZERO(&readSocketFd);
    FD_SET(sockfd, &readSocketFd);
    while(1)
    {
    	char message[512];
        int newConnectionFd = -1;
        struct timeval intTime;
        intTime.tv_sec = 5;
    	memset(&message, 0, sizeof(message));
		int retSel = -1;
        socklen_t clientLen = (socklen_t) sizeof(client);
        
        /*newConnectionFd = accept(sockfd, (struct sockaddr *) &client, &clientLen);
        if (newConnectionFd == -1)
        {
        	perror("Accept error:");
        }
        debugS("Accepted connection");*/
        debugS("Before select()");
        //retSel = select(sockfd+1, &readSocketFd, 0, 0, 0);
        if ((retSel = select(sockfd+1, &readSocketFd, 0, 0, &intTime)) == -1)
        {
            perror("Select() error: ");
        }
        else 
        if (retSel > 0)
        {
            debugS("Retsel > 0");
            for (int nextSock = 0; nextSock < FD_SETSIZE; nextSock++) //FD_SETSIZE = 1024 man select()
            {
                if (FD_ISSET(nextSock, &readSocketFd))
                {
                    if (nextSock == sockfd)
                    {
                        debugS("rusl");
                        FD_ISSET(sockfd, &readSocketFd);
                        newConnectionFd = accept(nextSock, (struct sockaddr*) &client, &clientLen);
                        char message[512];
                        memset(&message, 0, 512);
                        int numberOfBytes = recv(nextSock, &message, 512, 0);
                        debugD("Number of bytes from client = ", numberOfBytes);

                        FD_SET(newConnectionFd, &readSocketFd);
                    }
                }
                /*else
                {
                    shutdown(nextSock, SHUT_RDWR);
                    close(nextSock);
                }*/
            }
        }
        else
        {
            debugS("Time expired");
        }
        debugS("After retsel");
	}
	return -1;
}

int checkIfNewClient(int sockfd, fd_set* readSocketFd, struct sockaddr_in *client, int clientLen)
{
	if (FD_ISSET(sockfd, readSocketFd))
	{
		return 0;
	}
	return 0;
}