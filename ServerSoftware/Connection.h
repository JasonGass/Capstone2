#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <thread>
#include "ReceiverHandler.h"

class ReceiverHandler;
class Connection
{
	private:
		int socketFD;
		std::thread receiveThread;
	public:
		Connection(int *socketFD);
		Connection(const Connection& con);
		~Connection();
		int* getSocketFD();
		Connection & operator = (const Connection &con);
};

#endif

