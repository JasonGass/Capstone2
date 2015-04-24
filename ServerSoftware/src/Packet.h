#ifndef PACKET_H
#define PACKET_H

#include <string>

struct Packet
{
	int socketFD;
	std::string message;
};

#endif
