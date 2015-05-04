#include "Receiver.h"
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include "errno.h"


Receiver::Receiver()
{

}

void Receiver::ReceiverThreadTask(int socketFD)
{
	char buf[512];
	int rc;
	bool connectionOpened = true;
	while(connectionOpened)
	{
		rc = recv(socketFD, buf, 512, 0);
		switch(rc)
		{
			case -1:
			{
				//message = "Error: Receive error";
				perror(NULL);
				break;
			}
			case 0:
			{
				//message = "Error: Connection closed";
				connectionOpened = false;
				break;
			}
			default:
			{
				Packet packet;
				packet.socketFD = socketFD;
				buf[rc] = '\0';
				packet.message = std::string(buf);
				incomingMessages.push(packet);
				std::cout<<packet.message<<std::endl;
			}
		}
	}
}

void Receiver::createNewReceiverThread(int socketFD)
{
	std::thread receiverThread(&Receiver::ReceiverThreadTask,this,socketFD);
	receiverThread.detach();
}

std::queue<Packet> Receiver::getIncomingMessages()
{
	return incomingMessages;
}

int Receiver::size()
{
	return incomingMessages.size();
}

Packet Receiver::front()
{
	return incomingMessages.front();
}

void Receiver::pop()
{
	incomingMessages.pop();
}

bool Receiver::empty()
{
	return incomingMessages.empty();
}
