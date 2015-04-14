#include "Receiver.h"
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include "errno.h"


Receiver::Receiver()
{

}

void Receiver::ReceiverThreadTask(Connection connection)
{
	char buf[512];
	std::string message;
	int rc;
	bool connectionOpened = true;
	while(connectionOpened)
	{
		rc = recv(connection.socketFD, buf, 512, 0);
		switch(rc)
		{
			case -1:
			{
				message = "Error: Receive error";
				perror(NULL);
				break;
			}
			case 0:
			{
				message = "Error: Connection closed";
				connectionOpened = false;
				break;
			}
			default:
			{
				message = connection.deviceID + ":" + std::string(buf);
				Packet packet;
				packet.connection = connection;
				packet.message = message;
				incomingMessages.push(packet);
			}
		}
	}
}

void Receiver::createNewReceiverThread(Connection connection)
{
	std::thread receiverThread(&Receiver::ReceiverThreadTask,this,connection);
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
