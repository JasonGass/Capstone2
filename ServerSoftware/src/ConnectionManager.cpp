#include "ConnectionManager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <thread>
#include <algorithm>

int defaultDeviceID = 1;
bool is_Integer(const std::string& s)
{
        return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c);}) == s.end();
}

ConnectionManager::ConnectionManager()
{
}
void ConnectionManager::linkSocketFDAndDeviceID(int socketFD, int deviceID)
{
	auto i = std::begin(connections);
	while( i != std::end(connections))
	{
		if (deviceID == i->deviceID || socketFD == i->socketFD)
		{
			i = connections.erase(i);
		}
		else
			++i;
	}
	connection newConnection;
	newConnection.socketFD = socketFD;
	newConnection.deviceID = deviceID;
	connections.push_back(newConnection);
}

void ConnectionManager::createNewConnection(int socketFD)
{
	connection newConnection;
	newConnection.socketFD = socketFD;
	newConnection.deviceID = defaultDeviceID;
	defaultDeviceID++;
	connections.push_back(newConnection);
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

void ConnectionManager::listConnections()
{
	std::cout<<"Current Connections"<<std::endl;
	for(int i = 0; i<connections.size(); i++)
	{
		std::cout<<"SocketFD: "<< connections.at(i).socketFD<<", DeviceID: "<<connections.at(i).deviceID<<std::endl;
	}
}
