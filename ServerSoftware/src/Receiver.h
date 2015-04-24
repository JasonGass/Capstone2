#ifndef RECEIVER_H
#define RECEIVER_H

#include <queue>
#include <string>
#include "Packet.h"

class Receiver
{
	private:
		std::queue<Packet> incomingMessages;	
	public:
		Receiver();
		void createNewReceiverThread(int socketFD);
		void ReceiverThreadTask(int socketFD);
		std::queue<Packet> getIncomingMessages();
		int size();
		Packet front();
		void pop();
		bool empty();
};

#endif 
