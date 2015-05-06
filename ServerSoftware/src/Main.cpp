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

ConnectionListener cl;
ConnectionManager cm;
Sender sender;
Receiver rec;
DatabaseManager dbm;

/******************************************
 * Splits a string by delimiter string into vector
 *****************************************/
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

/******************************************
 * Checks if it is safe to convert string to int
 *****************************************/
bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c);}) == s.end();
}

/******************************************
 * Returns a response based on the command received.
 *****************************************/
std::string respond(std::string message, int socketFD)
{
	std::vector<std::string> command = split(",", message);
	if (!is_number(command[0]))
		return "";
	int deviceID = std::stoi(command[0]);
	switch (command.size())
	{
		case 1:
			cm.linkSocketFDAndDeviceID(socketFD,deviceID);
			return "";
		case 3: 
			if(command[1] == "get" && command[2] == "sensorID")
			{
				return std::to_string(dbm.getSensorID(deviceID));
			}
			break;
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
				return "";
			}
			if(command[1] == "set" && command[2] == "motion" && command[3] == "delay" && is_number(command[4]))
			{	
				dbm.setMotionDelay(deviceID, std::stoi(command[4]));
				return "";
			}
			if(command[1] == "set" && command[2] == "motion" && command[3] == "current" && is_number(command[4]))
			{	
				dbm.setMotionStatus(deviceID, std::stoi(command[4]));
				return "set,"+std::to_string(dbm.getMotionDelay(deviceID));
			}
			if(command[1] == "set" && command[2] == "outlet" && command[3] == "status" && is_number(command[4]))
			{	
				dbm.setOutletStatus(deviceID, std::stoi(command[4]));
				return "";
			}
	}
	return "Invalid Command";
}

/******************************************
 * Task for sending console input to devices for debugging purposes
 *****************************************/
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

/******************************************
 * Main thread of execution.  
 *****************************************/
int main()
{
	std::thread writeThread(writeThreadTask);
	while(1){

		//While incoming connections
		while(!cl.empty())
		{
			cm.createNewConnection(cl.front());
			rec.createNewReceiverThread(cl.front());
			cl.pop();
		}

		//While incoming messages
		while(!rec.empty())
		{
			Packet packet;
			packet.socketFD = rec.front().socketFD;
			packet.message = respond(rec.front().message,rec.front().socketFD );
			if(packet.message != "")
			{
				sender.push(packet);
			}			
			rec.pop();
		}
		
		//Hack since there is only one mcu in our example
		int mcuID = 1001;

		//If control circuit is connected to MCU, send status
		if (dbm.getSensorID(mcuID) ==3 && cm.getSocketFD(mcuID) != 0)
		{
			Packet packet;
			packet.socketFD = cm.getSocketFD(mcuID);
			packet.message = "set,"+std::to_string(dbm.getOutletStatus(mcuID));
			sender.push(packet);
		}

		//If temperature circuit is connected to MCU, send either wakeup or get temperature command 
		if (dbm.getSensorID(mcuID) ==1 && cm.getSocketFD(mcuID) != 0)
		{
			int event = dbm.checkTemperature();
			Packet packet;
			packet.socketFD = cm.getSocketFD(mcuID);
			//Used to get temperature
			if(event == mcuID)
				packet.message = "get,temp";
			//Used to wake MCU so it is not stuck in blocking recv call
			else
				packet.message = "wake";
			sender.push(packet);
		}

		std::cout.flush();
		sleep(1);
	}
	return 0;
}
