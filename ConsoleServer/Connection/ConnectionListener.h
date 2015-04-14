#ifndef CONNECTIONLISTENER_H
#define CONNECTIONLISTENER_H
#include <queue>

class ConnectionListener
{
	private:
		int socketFD;
		std::queue<int> incomingConnections; 

	public:
		ConnectionListener();
		~ConnectionListener();
		void start();
		std::queue<int> getIncomingConnections();
		int front();
		void pop();
		bool empty();
		int size();
};

#endif

