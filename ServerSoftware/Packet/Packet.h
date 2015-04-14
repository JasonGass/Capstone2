
#ifndef CONNECTION_H
#define CONNECTION_H
struct Connection
{
	int socketFD;
	int deviceID;
};

#endif

#ifndef PACKET_H
#define PACKET_H

#include <string>


struct Packet
{
Connection connection;
std::string message;
};

#endif
