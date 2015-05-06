#ifndef SENDER_H
#define SENDER_H

#include <queue>
#include "Packet.h"

class Sender
{
	private:
		//Messages that are ready to send.
		std::queue<Packet> messagesToSend;

	public:
		Sender();
		void push(Packet packet);
		void senderThreadTask();
};

#endif
