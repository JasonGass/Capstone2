
#include "Connection.h"
#include "ReceiverHandler.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

bool ConnectionOpened;

int sendMessage(int socketFD,std::string message)
{
	send(socketFD, message.c_str(), message.length()+1,0);
}

void ReceiveThreadTask(int socketFD)
{
	ReceiverHandler receiver(5);
	char buf[512];
	std::string str;
	int rc;

	while(true)
	{
		rc = (recv(socketFD, buf, 512, 0));
		switch(rc)
		{
			case -1: 
			{
				str = "Error: Receive error";
				break;
			}
			case 0:
			{
				ConnectionOpened = false;
				str = "Error: Connection closed";
				std::cout<<str<<std::endl;
				return;
			}
			default:
			{
				str = std::string(buf);
				std::string reply= receiver.messageReceived(str);
				sendMessage(socketFD, reply);
			}
		}
		std::cout<<str<<std::endl;
	}
	return;
}

Connection::Connection(int *SFD):socketFD(*SFD)
{
	ConnectionOpened = true;
	this->receiveThread = std::thread(ReceiveThreadTask, socketFD);	
	std::string mes;
	char buf[512];
	while(ConnectionOpened)
	{
		std::cin>>mes;
		sendMessage(socketFD, mes);
	}
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


