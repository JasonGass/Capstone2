#ifndef PACKET_H
#define PACKET_H

#include <string>

//Message and socket field descriptor so message goes to correct socket.
struct Packet
{
	int socketFD;
	std::string message;
};

#endif
