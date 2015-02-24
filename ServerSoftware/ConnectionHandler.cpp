#include "ConnectionHandler.h"
#include "Connection.h"
#include <vector>

ConnectionHandler::ConnectionHandler(int y){
	//std::cout<<"ConnectionHandler"<<std::endl;
	this->x=y;
}

ConnectionHandler::ConnectionHandler()
{
this->x = 5;
}

void ConnectionHandler::addConnection(int *socketFD)
{
	std::cout<<"Added Connection"<<std::endl;
	Connection con(this,socketFD);
	Connections.push_back(std::move(con));
}

void ConnectionHandler::sendMessageToConnection(Connection connection, std::string message)
{
	connection.sendMessage(message);
}

void ConnectionHandler::messageReceived(Connection *connection, std::string message)
{
	std::cout<<connection->getSocketFD()<<":"<<message<<std::endl;
}
