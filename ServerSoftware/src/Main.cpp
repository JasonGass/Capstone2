#include "ConnectionListener.h"
#include "Receiver.h"
#include "Sender.h"
#include "DatabaseManager.h"
#include "ConnectionManager.h"
#include <unistd.h>
#include <vector>
#include <iostream>
#include <thread>
#include <algorithm>


ConnectionManager cm;
Sender sender;
DatabaseManager dbm;

std::vector<std::string> split(std::string delimiter, std::string str)
{
	std::vector<std::string> delimitedString;
	size_t pos = 0;
	std::string token;
	while((pos = str.find(delimiter)) != std::string::npos)
	{
		token = str.substr(0,pos);
		delimitedString.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	delimitedString.push_back(str);
	return delimitedString;
}

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c);}) == s.end();
}

std::string respond(std::string message, int id)
{
	std::vector<std::string> command = split(",", message);
	switch (command.size())
	{
		case 2:
			if(command[0] == "get" && command[1] == "sensor")
			{	
//				return std::to_string(dbm.getSensor(id));
			}
			else if(command[0] == "get" && command[1] == "rule")
			{	
//				return dbm.getRules();
			}
			break;
		case 3:
			if(command[0] == "add" && command[1] == "temp" && is_number(command[2])) 
			{	
//				dbm.addTemperatureData(id,std::stoi(command[2]));
//				return std::to_string(dbm.getInterval(id));
			}
			else if(command[0] == "get" && command[1] == "temp" && is_number(command[2]))
			{	
				int number = std::stoi(command[2]);
//				return dbm.getTemperatureRecent(id,std::stoi(command[2]));
			}
			else if(command[0] == "add" && command[1] == "motion" && is_number(command[2]))
			{	
////				dbm.addMotionChange(id,std::stoi(command[2]));
				return "ACK";
			}
			else if(command[0] == "get" && command[1] == "motion" && is_number(command[2]))
			{	
//				return dbm.getMotionChangesRecent(id,std::stoi(command[2]));
			}
			else if(command[0] == "set" && command[1] == "sensor" && is_number(command[2]))
			{	
//				dbm.changeSensor(id, std::stoi(command[2]));
				return "ACK";
			}
			break;
		case 6:
			if(command[0] == "add" && command[1] == "rule" && is_number(command[2])  && is_number(command[3]) && is_number(command[4]) && is_number(command[5]))
			{	
//				dbm.addRule(std::stoi(command[2]), std::stoi(command[3]),std::stoi(command[4]),std::stoi(command[5]));
				return "ACk";
			}
			else if(command[0] == "rem" && command[1] == "rule" && is_number(command[2])  && is_number(command[3]) && is_number(command[4]) && is_number(command[5]))
			{	
//				dbm.removeRule(std::stoi(command[2]), std::stoi(command[3]),std::stoi(command[4]),std::stoi(command[5]));
				return "ACk";
			}			
	}
	return "Invalid Command";
}

void writeThreadTask()
{
	std::string message;
	while (1)
	{
		std::cin>>message;
		if (message=="l")
		{
			cm.listConnections();
		}
		if (message.substr(0,1)=="d")
		{
//			dbm.printDatabase(std::stoi(message.substr(1)));
		}
		else if (message.find(":")!=std::string::npos)
		{
			Packet packet;
			packet.socketFD = cm.getSocketFD(std::stoi(message.substr(0,message.find(":"))));
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
			cm.createNewConnection(cl.front());
			cl.pop();
		}
		while(!cm.empty())
		{
			//dbm.isConnected(cm.getDeviceID(cm.front()),true);
			rec.createNewReceiverThread(cm.front());
			cm.pop();
		}
		while(!rec.empty())
		{
			int id = cm.getDeviceID(rec.front().socketFD);
			Packet packet;
			packet.socketFD = rec.front().socketFD;
			packet.message = respond(rec.front().message, id);;
			sender.push(packet);			
			std::cout<<id<<":"<<rec.front().message<<std::endl;
			rec.pop();
		}
	//	std::string event = dbm.checkForUpdates();
	//	if(event != "")
	//	{
		//	packet
	//	}
		std::cout.flush();
		sleep(1);
	}
	return 0;
}
