#ifndef SENDER_H
#define SENDER_H

#include <queue>
#include "../Packet/Packet.h"

class Sender
{
	private:
		std::queue<Packet> messagesToSend;

	public:
		Sender();
		void push(Packet packet);
		void senderThreadTask();
};

#endif
