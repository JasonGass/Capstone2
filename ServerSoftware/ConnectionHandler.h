#ifndef CONNECTIONHANDLER_H 
#define CONNECTIONHANDLER_H

#include <vector>
#include "Connection.h"

class Connection;

class ConnectionHandler
{
	private:
		std::vector<Connection> Connections; 
		int x;
	public:
		ConnectionHandler(int y);
		ConnectionHandler();
		void addConnection(int *socketFD);
		void sendMessageToConnection(Connection connection, std::string message);
		void messageReceived(Connection *connection, std::string message);
};

#endif	

