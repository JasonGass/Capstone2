
#include "Connection.h"
#include "ConnectionHandler.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

void ReceiveThreadTask(Connection *connection, ConnectionHandler *handle)
{
	char buf[512];
	std::string str;
	int rc;
	int socketFD = *connection->getSocketFD(); 

	while(true)
	{
		rc = recv(socketFD, buf, 512, 0);
		switch(rc)
		{
			case -1: 
			{
				str = "Error: Receive error";
			}
			case 0:
			{
				str = "Error: Connection closed";
				return;
			}
			default:
			{
				str = std::string(buf);
			}
		}
	//	std::cout<<str<<std::endl;
		handle->messageReceived(connection,str);
	}
	return;
}

Connection::Connection(ConnectionHandler *newHandler, int *SFD):socketFD(*SFD), handler(newHandler)
{
	this->receiveThread = std::thread(ReceiveThreadTask, this, handler);	
	std::string mes;
	while(true)
	{
		std::cin>>mes;
		this->sendMessage(mes);
	}
}

Connection::Connection(const Connection& con)
{
	handler = new ConnectionHandler;
	*handler = *con.handler;
}

Connection & Connection::operator = (const Connection &con)
{
	if(this != &con)
		*handler = *(con.handler);
	return *this;
}

Connection::~Connection()
{
	this->receiveThread.join();
	close(this->socketFD);
}


int* Connection::getSocketFD()
{
	return &socketFD;
}

int Connection::sendMessage(std::string message)
{
	send(socketFD, message.c_str(), message.length()+1,0);
}

