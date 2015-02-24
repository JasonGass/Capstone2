#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include "ConnectionHandler.h"

#define MAXHOSTNAME 256
#define PORTNUMBER 5002
#define MAXNUMBEROFCONNECTIONS 5

void handleSig(int signum)
{
	while(waitpid(-1,NULL,WNOHANG) > 0);
}

int error(std::string message, int *socketFD, int *socketConnectionFD)
{
	std::cout<<message<<std::endl;
	close(*socketFD);
	close(*socketConnectionFD);
	return 1;
}

int main()
{
	ConnectionHandler handler(5);

	int socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(socketFD < 0)
		return error("Error: Cannot create socket", &socketFD, NULL);	
	
	struct sockaddr_in socketAddress;
	char sysHost[MAXHOSTNAME+1];

	bzero(&socketAddress, sizeof(sockaddr_in));

	
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddress.sin_port = htons(PORTNUMBER);

	int bindResult = bind(socketFD, 
			 		 (struct sockaddr *) &socketAddress,
					 sizeof(struct sockaddr_in));
	if(bindResult<0)
		return error("Error: Failure to bind (Port open)",&socketFD, NULL);

	listen(socketFD, MAXNUMBEROFCONNECTIONS);

	while(true)
	{
		int socketConnection = accept(socketFD, NULL, NULL);
		if (socketConnection < 0)
			return error("Error: Failure to connect", &socketFD, &socketConnection);

		switch(fork())
		{
			case -1:
			{
				return error("Error: Fork Error",&socketFD,&socketConnection);
			}
			case 0:
			{
				handler.addConnection(&socketConnection);
				return 0;
			}
			default:
			{
				close(socketConnection);
				continue;
			}
		}
	}
	close(socketFD);
	return 0;
}

