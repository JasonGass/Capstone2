#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <thread>
#include "ConnectionHandler.h"

class ConnectionHandler;
class Connection
{
	private:
		int socketFD;
		ConnectionHandler *handler;
		std::thread receiveThread;
	public:
		Connection(ConnectionHandler * newHandler, int *socketFD);
		Connection(const Connection& con);
		~Connection();
		int* getSocketFD();
		int sendMessage(std::string message);
		Connection & operator = (const Connection &con);
};

#endif

