#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <stdexcept>
#include "ConnectionListener.h"
#include "Connection.h"

#define MAXHOSTNAME 256
#define PORTNUMBER 5002
#define MAXNUMBEROFCONNECTIONS 5

void handleSig(int signum)
{
	while(waitpid(-1,NULL,WNOHANG) > 0);
}

ConnectionListener::ConnectionListener()
{
	socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(socketFD < 0)
	{
		close(socketFD);
		throw std::invalid_argument("Error: Cannot create socket");
	}
	int iSetOption = 1;
	setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
	
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
	{
		close(socketFD);
		throw std::invalid_argument("Error: Cannot bind");
	}

	listen(socketFD, MAXNUMBEROFCONNECTIONS);

	while(true)
	{
		int socketConnection = accept(socketFD, NULL, NULL);
		if (socketConnection < 0)
		{
			close(socketFD);
			close(socketConnection);
			throw std::invalid_argument("Error: Failure to connect");
		}

		switch(fork())
		{
			case -1:
			{
				close(socketFD);
				close(socketConnection);
				throw std::invalid_argument("Error: Fork Error");
			}
			case 0:
			{
				Connection con(&socketConnection);
			}
			default:
			{
				close(socketConnection);
				continue;
			}
		}
	}
	close(socketFD);
}

ConnectionListener::~ConnectionListener()
{
	close(socketFD);
}

