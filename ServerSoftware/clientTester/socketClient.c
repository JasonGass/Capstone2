#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <pthread.h>
#define MAXHOSTNAME 256
using namespace std;

void *Receive(void * socket)
{
	int rc;
	char buf[512];
	intptr_t socketConnection = (intptr_t) socket;
	while(1)
	{
		rc = recv(socketConnection, buf, 512, 0);
//		std::cout<<"receiving: "<<rc<<std::endl;
		if(rc > 0)
		{
			buf[rc] = (char) NULL;
			std::cout<<"Received: "<<buf<<std::endl;
		}
	}	
}

main()
{
	struct sockaddr_in remoteSocketInfo;
	struct hostent *hPtr;
	int socketHandle;
	const char *remoteHost="localhost";
	int portNumber = 5002;

	bzero(&remoteSocketInfo, sizeof(sockaddr_in));  // Clear structure memory
			 
			   // Get system information
			 
	if((hPtr = gethostbyname(remoteHost)) == NULL)
	{
		cerr << "System DNS name resolution not configured properly." << endl;
		cerr << "Error number: " << ECONNREFUSED << endl;
		exit(EXIT_FAILURE);
	}
	 
	   // create socket
	 
	if((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	close(socketHandle);
	exit(EXIT_FAILURE);
	}

  // Load system information into socket data structures

   memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
   remoteSocketInfo.sin_family = AF_INET;
   remoteSocketInfo.sin_port = htons((u_short)portNumber);      // Set port number
 
   if(connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)) < 0)
   {
   close(socketHandle);
   exit(EXIT_FAILURE);
   }
   pthread_t receiveThread;
	 pthread_create(&receiveThread, NULL,Receive, (void *) (    intptr_t) socketHandle);
	while(1)
	{
		int rc = 0;  // Actual number of bytes read by function read()
		char buf[512];

//		rc = recv(socketHandle,buf,512,0);
//		buf[rc]=(char)NULL;
		
		//std::cout<<"Data Sent"<<std::endl;
	//	strcpy(buf,"Message to send 2 test");
	//	send(socketHandle, buf, strlen(buf)+1, 0);
		//				sleep(1);
		std::cin>>buf;
		send(socketHandle, buf, strlen(buf)+1, 0); 
	}
}
