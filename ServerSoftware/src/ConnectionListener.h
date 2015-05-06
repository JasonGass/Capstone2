#ifndef CONNECTIONLISTENER_H
#define CONNECTIONLISTENER_H
#include <queue>

class ConnectionListener
{
	private:
		//Socket to listen on
		int socketFD;
		//Connections that are ready to move on.
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

