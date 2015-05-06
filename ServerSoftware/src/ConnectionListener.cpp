#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <stdexcept>
#include "ConnectionListener.h"
#include <vector>
#include <thread>

#define MAXHOSTNAME 256
#define PORTNUMBER 5002
#define MAXNUMBEROFCONNECTIONS 5

void handleSig(int signum)
{
	while(waitpid(-1,NULL,WNOHANG) > 0);
}


/****************************
 * Starts the Connection Listener
 ****************************/
void ConnectionListener::start()
{
	//Creates socket.  Assigns a Socket FD for listening.
	socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(socketFD < 0)
	{
		close(socketFD);
		throw std::invalid_argument("Error: Cannot create socket");
	}
	//Sets options for socket to listen on
	int iSetOption = 1;
	setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
	
	struct sockaddr_in socketAddress;
	char sysHost[MAXHOSTNAME+1];

	bzero(&socketAddress, sizeof(sockaddr_in));

	//Sets IP address and port number to listen on.
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddress.sin_port = htons(PORTNUMBER);

	//Binds the socket to the address
	int bindResult = bind(socketFD, 
			 		 (struct sockaddr *) &socketAddress,
					 sizeof(struct sockaddr_in));
	if(bindResult<0)
	{
		close(socketFD);
		throw std::invalid_argument("Error: Cannot bind");
	}

	//Starts listening for incoming connections
	listen(socketFD, MAXNUMBEROFCONNECTIONS);

	//Loop that accepts new incoming connections and adds them to queue
	while(true)
	{
		int socketConnection = accept(socketFD, NULL, NULL);
		if (socketConnection < 0)
		{
			close(socketFD);
			close(socketConnection);
			throw std::invalid_argument("Error: Failure to connect");
		}

		incomingConnections.push(socketConnection);
	}
	close(socketFD);
}

/****************************
 * Constructor: Starts thread that runs start() function and detachs thread.
 ****************************/
ConnectionListener::ConnectionListener()
{
	std::thread listenThread(&ConnectionListener::start,this);
	listenThread.detach();
}

/****************************
 * Destructor: closes the socket it is listening on.
 ****************************/
ConnectionListener::~ConnectionListener()
{
	close(socketFD);
}

/****************************
 * Gets the queue of connections that are ready to move on.
 ****************************/
std::queue<int> ConnectionListener::getIncomingConnections()
{
	return incomingConnections;
}

/****************************
 * Gets first entry in queue of connections that are ready to move on.
 ****************************/
int ConnectionListener::front()
{
	return incomingConnections.front();
}

/****************************
 * Removes first entry in queue of connections that are ready to move on.
 ****************************/
void ConnectionListener::pop()
{
	incomingConnections.pop();
}

/****************************
 * Checks if queue of connections that are ready to move on is empty.
 ****************************/
bool ConnectionListener::empty()
{
	return	incomingConnections.empty();
}

/****************************
 * Gets number of connections that are ready to move on.
 ****************************/
int ConnectionListener::size()
{
	return incomingConnections.size();
}



