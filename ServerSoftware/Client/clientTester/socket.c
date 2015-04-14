#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>

#define MAXHOSTNAME 256

void handleSig(int signum)
{
	while(waitpid(-1,NULL,WNOHANG) >0);
}

void *Receive(void * socket)
{
	int rc;
	char buf[512];
	intptr_t socketConnection = (intptr_t) socket;
	while(1)
	{
		rc = recv(socketConnection, buf, 512, 0);
		if(rc > 0)
		{
			buf[rc] = (char) NULL;
			std::cout<<"Received: "<<buf<<std::endl;
		}
	}
}

int main()
{
	int socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (socketHandle < 0)
	{
		std::cout<<"Failure"<<std::endl;
		close(socketHandle);
		return 1;
	}
	std::cout<<"Success"<<std::endl;

	struct sockaddr_in socketAddress;
	char sysHost[MAXHOSTNAME+1];
////	struct hostent *hPtr;
	int portNumber = 5002;

	bzero(&socketAddress, sizeof(sockaddr_in));
	
//	gethostname(sysHost, MAXHOSTNAME);
//	if((hPtr = gethostbyname(sysHost)) == NULL)
//	{
//		std::cerr << "System hostname misconfigured"<<std::endl;
//		return 1;
//	}

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	socketAddress.sin_port = htons(portNumber);

	int bindResult =  bind(socketHandle, 
			(struct sockaddr *) &socketAddress, 
			sizeof(struct sockaddr_in));
	if(bindResult<0)
	{
		std::cout<<"Failure to bind"<<std::endl;
		close(socketHandle);
		return 1;
	}

	listen(socketHandle, 5);

	while(1)
	{
		int socketConnection = accept(socketHandle, NULL, NULL);
		if (socketConnection < 0)
		{
			close(socketHandle);
			if(errno==EINTR) continue;
			std::cout<<"Failure to Connect"<<std::endl;
			return 1;
		}
		int rc,sd;
		char buf[512];
		pthread_t receiveThread;

		switch(fork())
		{
			case -1: 
				std::cout<<"Fork Error"<<std::endl;
				close(socketHandle);
				close(socketConnection);
				return 1;
			case 0:
				pthread_create(&receiveThread, NULL,Receive, (void *) (intptr_t) socketConnection);
				while(1)
				{
					
					rc =0;
					char buf[512];
					std::cin>>buf;
					send(socketConnection, buf, strlen(buf)+1, 0);
				}
				close(socketHandle);
				return 0;
			default:
				close(socketConnection);
				continue;
		}
	}	
	close(socketHandle);
	return 0;
}

