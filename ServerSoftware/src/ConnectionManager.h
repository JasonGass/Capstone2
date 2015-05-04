#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <vector>

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
		std::vector<connection> connections;
};

#endif

