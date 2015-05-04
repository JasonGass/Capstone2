#include "DatabaseManager.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>


struct tm getTime()
{
	time_t t = time(0);
	return *localtime(&t);
}

bool updateOutlet = false; 

DatabaseManager::DatabaseManager()
{
	Device device;
	device.deviceID = 1000;
	database.push_back(device);
	Device device2;
	device2.deviceID = 1001;
	database.push_back(device2);
	Device device3;
	device3.deviceID = 1002;
	database.push_back(device3);
	loadDatabase();
	lastRead = time(NULL);
}

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
	updateOutlet=true;
	saveDatabase();
}

void DatabaseManager::setOutletRule(int outlet_deviceID, int sensor_deviceID, int sensor, int value, int comparator)
{
	database[outlet_deviceID-1000].outlet_rule_deviceID = sensor_deviceID;
	database[outlet_deviceID-1000].outlet_rule_value = value;
	database[outlet_deviceID-1000].outlet_rule_sensor = sensor;
	database[outlet_deviceID-1000].outlet_rule_comparator = comparator;
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
int DatabaseManager::getOutletRule_deviceID(int deviceID) 
{
	return database[deviceID-1000].outlet_rule_deviceID;;
}
int DatabaseManager::getOutletRule_sensor(int deviceID) 
{
	return database[deviceID-1000].outlet_rule_sensor;;
}
int DatabaseManager::getOutletRule_value(int deviceID) 
{
	return database[deviceID-1000].outlet_rule_value;;
}
int DatabaseManager::getOutletRule_comparator(int deviceID) 
{
	return database[deviceID-1000].outlet_rule_comparator;;
}
			     
int DatabaseManager::checkRules() 
{
	for(auto &it : database)
	{
		if(it.sensorID == OUTLET)
		{
			if (updateOutlet)
			{
				updateOutlet = false;
				return it.deviceID;
			}
			/*
			if (database[it.outlet_rule_deviceID-1000].sensorID != it.outlet_rule_sensor)
				break;
			int value;
			if (it.outlet_rule_sensor == TEMPERATURE)
				value = database[it.outlet_rule_deviceID-1000].temperature_current;
			else if (it.outlet_rule_sensor == MOTION)
				value = database[it.outlet_rule_deviceID-1000].motion_status;;
			
			bool comparison= false;
			if(it.outlet_rule_comparator == GREATERTHAN)
			{
				comparison = (it.outlet_rule_value > value);
			}
			else if(it.outlet_rule_comparator == LESSTHAN)
			{
				comparison = (it.outlet_rule_value < value);
			}
			
			if (comparison != it.outlet_on)
			{
				it.outlet_on = comparison;
				return it.deviceID;
			}
			*/
			return 0;
		}
	}
}

int DatabaseManager::checkTemperature()
{
	for (auto &it : database)
	{
		if (it.sensorID == TEMPERATURE)
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
				     
void DatabaseManager::printDatabase() 
{
}
void DatabaseManager::saveDatabase() 
{
	std::ofstream file;
	file.open("./bin/device_data.txt");
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
			file << it.outlet_rule_deviceID<<std::endl;
			file << it.outlet_rule_value<<std::endl;
			file << it.outlet_rule_sensor<<std::endl;
			file << it.outlet_rule_comparator<<std::endl;
		}
		file.close();
	}
}
void DatabaseManager::loadDatabase()
{
	std::ifstream file;
	int len;
	file.open("./bin/device_data.txt");
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
			file >> it.outlet_rule_deviceID;
			file >> it.outlet_rule_value;
			file >> it.outlet_rule_sensor;
			file >> it.outlet_rule_comparator;
		}
	}
}

