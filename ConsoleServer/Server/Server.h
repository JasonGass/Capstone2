#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <thread>
#include "../Connection/ConnectionListener.h"
//#include "../Connection/Connection.h"

struct Connection{
	int socketFD;
	int deviceID;
};

class Server
{
	private:
		ConnectionListener connectionListener;
		std::vector<Connection> connections;
	public:
		Server(); 
		std::vector<Connection> getConnections();
};

#endif
