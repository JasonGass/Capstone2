#include <iostream>
#include "Server.h"
#include <thread>

void ConnectionListenerTask(ConnectionListener& connectionListener, std::vector<Connection>& connections)
{
	std::cout<<"ConnectionListenerTask started"<<std::endl;
	connectionListener.start(connections);
	std::cout<<"ConnectionListenerTask ended"<<std::endl;
}

void Server::startConnectionListener()
{
	std::cout<<"start started"<<std::endl;
	connectionListenerThread = std::thread(ConnectionListenerTask, std::ref(connectionListener), std::ref(connections));
	connectionListenerThread.detach();
	std::cout<<"start ended"<<std::endl;
}


std::vector<Connection> Server::getConnections()
{
	return connections;
}
