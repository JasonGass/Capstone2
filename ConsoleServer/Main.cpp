#include "./Connection/ConnectionListener.h"
#include "./Receiver/Receiver.h"
#include "./Sender/Sender.h"
#include <unistd.h>
#include <vector>
#include <iostream>
#include <thread>


std::vector<Connection> connections;
void writeThreadTask()
{
	Sender sender;
	std::string message;
	while (1)
	{
		std::cin>>message;
		if (message=="l")
		{
			std::cout<<"Device IDs: "<<std::endl;
			for(int i = 0; i < connections.size(); i++)
			{
				std::cout<<connections.at(i).deviceID<<std::endl;
			}
			std::cout<<std::endl;
		}
		else if (message.find(":")!=std::string::npos)
		{
			Packet packet;
			packet.connection.socketFD = std::stoi(message.substr(0,message.find(":")));
			packet.message = message.substr(message.find(":")+1);
			sender.push(packet);
		}
		else
		{	
			std::cout<<"Enter valid input, socketFD:Message"<<std::endl;
		}
	}
}

int main()
{
	ConnectionListener cl;
	Receiver rec;
	std::thread writeThread(writeThreadTask);
	int size;
	while(1){
		while(!cl.empty())
		{
			Connection newConnection;
			newConnection.socketFD = cl.front();
			newConnection.deviceID = cl.front();
			connections.push_back(newConnection);
			rec.createNewReceiverThread(newConnection);
			cl.pop();
		}
		while(!rec.empty())
		{
			std::cout<<rec.front().connection.socketFD<<":"<<rec.front().message<<std::endl;
			rec.pop();
		}
		std::cout.flush();
		sleep(1);
	}
	return 0;
}
