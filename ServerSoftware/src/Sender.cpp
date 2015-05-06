#include "Sender.h"
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>

/******************************
 * Thread to empty queue and send messages
 *****************************/
void Sender::senderThreadTask()
{
	while(1){
		while (!messagesToSend.empty())
		{
			messagesToSend.front().message += "\n";
			send(messagesToSend.front().socketFD, messagesToSend.front().message.c_str(), messagesToSend.front().message.length()+1, 0);
			std::cout<<"Sending: "<<messagesToSend.front().message.c_str()<<std::endl;

			messagesToSend.pop();
		}
	}
}

/******************************
 * Constructor: Creates new send thread and detachs from main thread.
 *****************************/
Sender::Sender()
{
	std::thread senderThread(&Sender::senderThreadTask, this);	
	senderThread.detach();
}

/******************************
 * Adds new message to send queue
 *****************************/
void Sender::push(Packet packet)
{
	messagesToSend.push(packet);
}
