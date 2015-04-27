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
	int deviceID = std::stoi(command[0]);
	switch (command.size())
	{
		case 3: 
			if(command[1] == "get" && command[2] == "sensorID")
			{
				return std::to_string(dbm.getSensorID(deviceID));
			}
		case 4:
			if(command[1] == "set" && command[2] == "sensorID" && is_number(command[3]))
			{	
				dbm.setSensorID(deviceID, std::stoi(command[3]));
				return "";
			}
			if(command[1] == "get" && command[2] == "temp" && command[3] == "interval")
			{	
				return std::to_string(dbm.getTemperatureInterval(deviceID));
			}
			if(command[1] == "get" && command[2] == "temp" && command[3] == "current")
			{	
				return std::to_string(dbm.getTemperatureCurrent(deviceID));;
			}
			if(command[1] == "get" && command[2] == "motion" && command[3] == "delay")
			{	
				return std::to_string(dbm.getMotionDelay(deviceID));;
			}
			if(command[1] == "get" && command[2] == "motion" && command[3] == "current")
			{	
				return std::to_string(dbm.getMotionStatus(deviceID));;
			}
			if(command[1] == "get" && command[2] == "outlet" && command[3] == "status")
			{	
				return std::to_string(dbm.getOutletStatus(deviceID));;
			}
			break;
		case 5:
			if(command[1] == "set" && command[2] == "temp" && command[3] == "interval" && is_number(command[4]))
			{	
				dbm.setTemperatureInterval(deviceID,std::stoi(command[4]));
				return "";
			}
			if(command[1] == "set" && command[2] == "temp" && command[3] == "current" && is_number(command[4]))
			{	
				dbm.setTemperatureCurrent(deviceID, std::stoi(command[4]));
				return std::to_string(dbm.getTemperatureInterval(deviceID));
			}
			if(command[1] == "set" && command[2] == "motion" && command[3] == "delay" && is_number(command[4]))
			{	
				dbm.setMotionDelay(deviceID, std::stoi(command[4]));
				return "";
			}
			if(command[1] == "set" && command[2] == "motion" && command[3] == "current" && is_number(command[4]))
			{	
				dbm.setMotionStatus(deviceID, std::stoi(command[4]));
				return std::to_string(dbm.getMotionDelay(deviceID));
			}
			if(command[1] == "set" && command[2] == "outlet" && command[3] == "status" && is_number(command[4]))
			{	
				dbm.setOutletStatus(deviceID, std::stoi(command[4]));
				return "";
			}
			if(command[1] == "get" && command[2] == "outlet" && command[3] == "rule" && command[4] == "deviceID")
			{	
				return std::to_string(dbm.getOutletRule_deviceID(deviceID));
			}
			if(command[1] == "get" && command[2] == "outlet" && command[3] == "rule" && command[4] == "value")
			{	
				return std::to_string(dbm.getOutletRule_value(deviceID));
			}
			if(command[1] == "get" && command[2] == "outlet" && command[3] == "rule" && command[4] == "sensor")
			{	
				return std::to_string(dbm.getOutletRule_sensor(deviceID));
			}
			if(command[1] == "get" && command[2] == "outlet" && command[3] == "rule" && command[4] == "comparator")
			{	
				return std::to_string(dbm.getOutletRule_comparator(deviceID));
			}
			break;
		case 8: 
			if(command[1] == "set" && command[2] == "outlet" && command[3] == "rule" && is_number(command[4]) && is_number(command[5]) && is_number(command[6]) && is_number(command[7]))
			{	
				dbm.setOutletRule(deviceID, 
								  std::stoi(command[4]),
								  std::stoi(command[5]),
								  std::stoi(command[6]),
								  std::stoi(command[7]));
				return "";
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
			if(packet.message != "")
				sender.push(packet);			
			std::cout<<id<<":"<<rec.front().message<<std::endl;
			rec.pop();
		}
		int event = dbm.checkRules();
		if(event > 999 && event < 1004 )
		{
			Packet packet;
			packet.socketFD = cm.getSocketFD(event);
			packet.message = std::to_string(dbm.getOutletStatus(event));
			sender.push(packet);
		}
		event = dbm.checkTemperature();
		if(event > 999 && event < 1004 )
		{
			Packet packet;
			packet.socketFD = cm.getSocketFD(event);
			packet.message = "get,temp";
			sender.push(packet);
		}
		std::cout.flush();
		sleep(1);
	}
	return 0;
}
