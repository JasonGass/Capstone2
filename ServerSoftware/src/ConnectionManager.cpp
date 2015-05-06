#include "ConnectionManager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <thread>
#include <algorithm>

//Device ID used for unknown devices
int defaultDeviceID = 1;

/*************************************
 * Checks if can be converted to an integer
 *************************************/
bool is_Integer(const std::string& s)
{
        return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c);}) == s.end();
}

/*************************************
 * Constructor
 *************************************/
ConnectionManager::ConnectionManager()
{
}

/*************************************
 * Links the socket FD to a device ID
 *************************************/
void ConnectionManager::linkSocketFDAndDeviceID(int socketFD, int deviceID)
{
	//Search for duplicate device IDs or socket FD
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

	//Adds the new connection entry to vector of connections
	connection newConnection;
	newConnection.socketFD = socketFD;
	newConnection.deviceID = deviceID;
	connections.push_back(newConnection);
}

/*************************************
 * Creates a new connection entry and adds to vector of connections
 *************************************/
void ConnectionManager::createNewConnection(int socketFD)
{
	connection newConnection;
	newConnection.socketFD = socketFD;
	newConnection.deviceID = defaultDeviceID;
	defaultDeviceID++;
	connections.push_back(newConnection);
	return;
}

/*************************************
 * Returns device ID of given socket FD
 *************************************/
int ConnectionManager::getDeviceID(int socketFD)
{
	for(int i = 0; i < connections.size(); i++)
	{
		if(socketFD == connections.at(i).socketFD)
			return connections.at(i).deviceID;
	}
	return 0;
}

/*************************************
 * Returns socket FD of given device ID
 *************************************/
int ConnectionManager::getSocketFD(int deviceID)
{
	for(int i = 0; i < connections.size(); i++)
	{
		if(deviceID == connections.at(i).deviceID)
			return connections.at(i).socketFD;
	}
	return 0;
}

/*************************************
 * Lists the current connections
 *************************************/
void ConnectionManager::listConnections()
{
	std::cout<<"Current Connections"<<std::endl;
	for(int i = 0; i<connections.size(); i++)
	{
		std::cout<<"SocketFD: "<< connections.at(i).socketFD<<", DeviceID: "<<connections.at(i).deviceID<<std::endl;
	}
}
