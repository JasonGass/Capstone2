#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <vector>
#include <queue>

struct connection{
	int socketFD;
	int deviceID;
};
class ConnectionManager
{
	public:
		ConnectionManager();
		void createNewConnection(int socketFD);
		void createNewConnectionTask(connection newConnection);
		int getDeviceID(int socketFD);
		int getSocketFD(int deviceID);
		bool empty();
		int front();
		void pop();
		void listConnections();
	private:
		std::vector<connection> connections;
		std::queue<int> finishedConnections;
};

#endif

