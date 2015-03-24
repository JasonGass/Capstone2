#include "ReceiverHandler.h"
#include "DataFile.h"

DataFile data("/home/server/Capstone2/ServerSoftware/DATA/data.txt");

ReceiverHandler::ReceiverHandler(int y){
	//Create/Connect to database
//	data = DataFile("/home/server/Capstone2/ServerSoftware/DATA/data.txt");
	x = y;
}

std::string ReceiverHandler::messageReceived(std::string message)
{
	char option = message[0];
	switch (option)
	{
		case 's':
		{
			data.addEntry(message.substring(1));
			return "ack";
		}
		case 'g':
		{
			return data.getPreviousEntries((int)message[1]-48);
		}
	}
	//do something with message
	return "ack";
}
