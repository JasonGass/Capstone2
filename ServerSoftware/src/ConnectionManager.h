#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <vector>

//Struct to make sure each socket FD has a device ID
struct connection{
	int socketFD;
	int deviceID;
};
class ConnectionManager
{
	public:
		ConnectionManager();
		void createNewConnection(int socketFD);
		void linkSocketFDAndDeviceID(int socketFD, int deviceID);
		int getDeviceID(int socketFD);
		int getSocketFD(int deviceID);
		void listConnections();
	private:
		//List of all the current connections
		std::vector<connection> connections;
};

#endif

