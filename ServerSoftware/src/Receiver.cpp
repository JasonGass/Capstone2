#include "Receiver.h"
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include "errno.h"


Receiver::Receiver()
{

}

/*********************************************
 * Task that is ran by receiver thread for each connection.
 ********************************************/
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
				//Parses incoming message by ';' into commands.
				//Adds command to incoming messages queue.
				buf[rc] = '\0';
				std::string message = std::string(buf);
				size_t pos = 0;
				while((pos = message.find(";")) != std::string::npos)
				{	
					Packet packet;
					packet.socketFD = socketFD;
					packet.message = message.substr(0,pos);;
					incomingMessages.push(packet);
					std::cout<<packet.message<<std::endl;
					message.erase(0,pos+1);
				}

			}
		}
	}
}


/*********************************************
 * Starts a new thread and detachs from main thread.
 ********************************************/
void Receiver::createNewReceiverThread(int socketFD)
{
	std::thread receiverThread(&Receiver::ReceiverThreadTask,this,socketFD);
	receiverThread.detach();
}

/*********************************************
 * Returns queue of messages that have been received
 ********************************************/
std::queue<Packet> Receiver::getIncomingMessages()
{
	return incomingMessages;
}

/*********************************************
 * Returns number of messages that have been received and are waiting to be processed.
 ********************************************/
int Receiver::size()
{
	return incomingMessages.size();
}

/*********************************************
 * Returns first received message.
 ********************************************/
Packet Receiver::front()
{
	return incomingMessages.front();
}

/*********************************************
 * Removes oldest received message.
 ********************************************/
void Receiver::pop()
{
	incomingMessages.pop();
}

/*********************************************
 * Checks if any new messages have been received.
 ********************************************/
bool Receiver::empty()
{
	return incomingMessages.empty();
}
