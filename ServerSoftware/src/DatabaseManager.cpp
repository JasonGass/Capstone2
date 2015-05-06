#include "DatabaseManager.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>

/*****************************
 * Returns current time
 ****************************/
struct tm getTime()
{
	time_t t = time(0);
	return *localtime(&t);
}

/*****************************
 * Constructor: Populates database vector then loads saved settings.
 ****************************/
DatabaseManager::DatabaseManager()
{
	for(int i = 0; i < NUMBER_OF_DEVICES; i++)
	{
		Device device;
		database.push_back(device);
	}
	loadDatabase();
	lastRead = time(NULL);
}

/*****************************
 * Getters and setters for the device variables.
 * Any unique lines are commented in function
 ****************************/
void DatabaseManager::setSensorID(int deviceID, int sensorID)
{
	if(sensorID < 4)
		database[deviceID-1000].sensorID = sensorID;		
	saveDatabase();
}

void DatabaseManager::setIsConnected(int deviceID, bool connected)
{
	database[deviceID-1000].isConnected = connected;
	saveDatabase();
}

void DatabaseManager::setTemperatureInterval(int deviceID, int interval)
{
	database[deviceID -1000].temperature_interval = interval;
	saveDatabase();
}

void DatabaseManager::setTemperatureCurrent(int deviceID, int temperature)
{
	//Adds timestamp so server knows when last data collection occured.
	lastRead = time(NULL);
	database[deviceID -1000].temperature_current = temperature;
	struct tm time = getTime();
	char buffer[100];	
	strftime(buffer,100,"%d-%m-%Y,%I:%M:%S", &time);
	std::string timeString(buffer);
	database[deviceID -1000].temperature_timeSinceReading = timeString;
	saveDatabase();
}

void DatabaseManager::setMotionDelay(int deviceID, int delay)
{
	database[deviceID -1000].motion_delay = delay;
	saveDatabase();
}

void DatabaseManager::setMotionStatus(int deviceID, bool motion)
{
	//Adds timestamp so server knows when last motion change occured.
	database[deviceID -1000].motion_status = motion;
	struct tm time = getTime();
	char buffer[100];	
	strftime(buffer,100,"%d-%m-%Y,%I:%M:%S", &time);
	std::string timeString(buffer);
	database[deviceID -1000].motion_timeSinceChange = timeString;	
	saveDatabase();
}

void DatabaseManager::setOutletStatus(int deviceID, bool powerOn)
{
	database[deviceID-1000].outlet_on = powerOn;
	saveDatabase();
}

int DatabaseManager::getSensorID(int deviceID)
{
	return database[deviceID-1000].sensorID;	
}

bool DatabaseManager::getIsConnected(int deviceID)
{
	return database[deviceID-1000].isConnected;	
}

int DatabaseManager::getTemperatureInterval(int deviceID) 
{
	return database[deviceID-1000].temperature_interval;;	
}
int DatabaseManager::getTemperatureCurrent(int deviceID) 
{
	return database[deviceID-1000].temperature_current;;	
}
std::string DatabaseManager::getTemperatureTime(int deviceID) 
{
	return database[deviceID-1000].temperature_timeSinceReading;	
}
int DatabaseManager::getMotionDelay(int deviceID) 
{
	return database[deviceID-1000].motion_delay;
}
bool DatabaseManager::getMotionStatus(int deviceID) 
{
	return database[deviceID-1000].motion_status;
}
std::string DatabaseManager::getMotionStatusTime(int deviceID) 
{
	return database[deviceID-1000].motion_timeSinceChange;	
}
bool DatabaseManager::getOutletStatus(int deviceID) 
{
	return database[deviceID-1000].outlet_on;;
}

/**********************************
 * Checks is temperature needs to be updated.
 **********************************/
int DatabaseManager::checkTemperature()
{
	for (auto &it : database)
	{
		if (it.sensorID == TEMPERATURE && it.deviceID == 1001)
		{
			time_t now;
			now = time(NULL);
			if(std::difftime(now, lastRead) > it.temperature_interval)
			{
				return it.deviceID;
			}
			else 
				return 0;
		}
	}
}
				     

/**********************************
 * Saves database vector's device variables to file.
 **********************************/
void DatabaseManager::saveDatabase() 
{
	std::ofstream file;
	file.open("./device_data.txt");
	if (file.is_open())
	{
		for(auto &it : database)
		{
			file << it.deviceID<<std::endl;
			file << it.sensorID<<std::endl;
			file << it.isConnected<<std::endl;
			file << it.temperature_interval<<std::endl;
			file << it.temperature_current<<std::endl;
			file << it.temperature_timeSinceReading<<std::endl;
			file << it.motion_delay<<std::endl;
			file << it.motion_status<<std::endl;
			file << it.motion_timeSinceChange<<std::endl;
			file << it.outlet_on<<std::endl;
		}
		file.close();
	}
}

/**********************************
 * Loads the database vector with device variables read in from file.
 **********************************/
void DatabaseManager::loadDatabase()
{
	std::ifstream file;
	int len;
	file.open("./device_data.txt");
	if (file.is_open())
	{
		for(auto &it : database)
		{
			file >> it.deviceID;
			file >> it.sensorID;
			file >> it.isConnected;
			file >> it.temperature_interval;
			file >> it.temperature_current;
			getline(file,it.temperature_timeSinceReading);
			getline(file,it.temperature_timeSinceReading);
			file >> it.motion_delay;
			file >> it.motion_status;
			getline(file,it.motion_timeSinceChange);
			getline(file,it.motion_timeSinceChange);
			file >> it.outlet_on;
		}
	}
}

