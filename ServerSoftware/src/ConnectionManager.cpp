#include "ConnectionManager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <thread>

ConnectionManager::ConnectionManager()
{
}
void ConnectionManager::createNewConnectionTask(connection newConnection)
{
	char buf[512];
	std::string message;
	int rc, deviceID;
	rc = recv(newConnection.socketFD, buf, 512, 0);
	message = std::string(buf);
	deviceID = stoi(message);
	send(newConnection.socketFD, buf, 512, 0);
	for(int i = 0; i < connections.size(); i++)
	{
		if (deviceID == connections.at(i).deviceID)
		{
			connections.at(i).socketFD = newConnection.socketFD;
			return;
		}
	}
	newConnection.deviceID = deviceID;
	connections.push_back(newConnection);
	finishedConnections.push(newConnection.socketFD);
}

void ConnectionManager::createNewConnection(int socketFD)
{
	connection newConnection;
	newConnection.socketFD = socketFD;
	newConnection.deviceID = -1;
	std::thread newConnectionThread(&ConnectionManager::createNewConnectionTask, this, newConnection);
	newConnectionThread.detach();
	return;
}

int ConnectionManager::getDeviceID(int socketFD)
{
	for(int i = 0; i < connections.size(); i++)
	{
		if(socketFD == connections.at(i).socketFD)
			return connections.at(i).deviceID;
	}
	return 0;
}

int ConnectionManager::getSocketFD(int deviceID)
{
	for(int i = 0; i < connections.size(); i++)
	{
		if(deviceID == connections.at(i).deviceID)
			return connections.at(i).socketFD;
	}
	return 0;
}

bool ConnectionManager::empty()
{
	return finishedConnections.empty();
}

int ConnectionManager::front()
{
	return finishedConnections.front();
}

void ConnectionManager::pop()
{
	finishedConnections.pop();
	return;
}

void ConnectionManager::listConnections()
{
	std::cout<<"Current Connections"<<std::endl;
	for(int i = 0; i<connections.size(); i++)
	{
		std::cout<<connections.at(i).deviceID<<std::endl;
	}
}
