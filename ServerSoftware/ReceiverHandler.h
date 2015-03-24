#ifndef RECEIVERHANDLER_H
#define RECEIVERHANDLER_H

#include <string>

class ReceiverHandler
{
	private:
		int x;

	public:
		ReceiverHandler(int y);
		std::string messageReceived(std::string message);
		
};

#endif


