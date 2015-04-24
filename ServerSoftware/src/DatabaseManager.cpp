#include "DatabaseManager.h"
#include <iostream>
#include <thread>
#include <iomanip>


struct tm getTime()
{
	time_t t = time(0);
	return *localtime(&t);
}

DatabaseManager::DatabaseManager()
{
	loadDatabase();
	if(database.size() != 3)
		std::cout<<"Load failed: # of devices loaded was not 3"<<std::endl;
}

void DatabaseManager::setSensorID(int deviceID, int sensorID)
{
	if(sensorID < 4)
		database[deviceID-1000].sensorID = sensorID;		
}

void DatabaseManager::setIsConnected(int deviceID, bool connected)
{
	database[deviceID-1000].isConnected = connected;
}

void DatabaseManager::setTemperatureInterval(int deviceID, int interval)
{
	database[deviceID -1000].temperature_interval = interval;
}

void DatabaseManager::setTemperatureCurrent(int deviceID, int temperature)
{
	database[deviceID -1000].temperature_current = temperature;
	database[deviceID -1000].temperature_timeSinceReading = getTime();
}

void DatabaseManager::setMotionDelay(int deviceID, int delay)
{
	database[deviceID -1000].motion_delay = delay;
}

void DatabaseManager::setMotionStatus(int deviceID, bool motion)
{
	database[deviceID -1000].motion_status = motion;
	database[deviceID - 1000].motion_timeSinceChange = getTime();
}

void DatabaseManager::setOutletStatus(int deviceID, bool powerOn)
{
	database[deviceID-1000].outlet_on = powerOn;
}

void DatabaseManager::setOutletRule(int outlet_deviceID, int sensor_deviceID, int sensor, int value, int comparator)
{
	database[outlet_deviceID-1000].outlet_rule_deviceID = sensor_deviceID;
	database[outlet_deviceID-1000].outlet_rule_value = value;
	database[outlet_deviceID-1000].outlet_rule_sensor = sensor;
	database[outlet_deviceID-1000].outlet_rule_comparator = comparator;
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
std::string DatabaseManager::getTemperatureCurrentString(int deviceID) 
{
	
	char buffer[100];	
	strftime(buffer,100,"%d-%m-%Y %I:%M:%S", &database[deviceID-1000].temperature_timeSinceReading);
	std::string timeString(buffer);
	return std::to_string(database[deviceID-1000].temperature_current) + ","+timeString;	
}
int DatabaseManager::getMotionDelay(int deviceID) 
{
	return database[deviceID-1000].motion_delay;
}
bool DatabaseManager::getMotionStatus(int deviceID) 
{
	return database[deviceID-1000].motion_status;
}
std::string DatabaseManager::getMotionStatusString(int deviceID) 
{
	char buffer[100];	
	strftime(buffer,100,"%d-%m-%Y %I:%M:%S", &database[deviceID-1000].motion_timeSinceChange);
	std::string timeString(buffer);
	return std::to_string(database[deviceID-1000].motion_status) + ","+timeString;	
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
			     
bool DatabaseManager::checkRules() 
{

}
				     
void DatabaseManager::printDatabase() 
{

}
void DatabaseManager::saveDatabase() 
{

}
void DatabaseManager::loadDatabase()
{

}

/*
int DatabaseManager::validateRule(Rule rule)
{
int current;
bool isTrue;
int index = findDatabaseIndex(rule.deviceID);
if (index < 0) 
return -1;
if (rule.attribute != getSensor(rule.deviceID))
return -1;
if (rule.attribute == TEMP)
current = getTemperatureCurrent(rule.deviceID);
if (rule.attribute == MOTION)
current = getMotionChangesCurrent(rule.deviceID);
if (rule.comparison == GREATERTHAN)
isTrue = current > rule.value;	
if (rule.comparison == LESSTHAN)
isTrue = current < rule.value;	
if (isTrue)
return 1;
else
return 0;
}

std::string DatabaseManager::checkForUpdates()
{
int controlID = this->findControlIndex();
if (controlID < 0)
return "";
int index = findDatabaseIndex(controlID);

bool controlON = true;

for(int i = 0; i<rules.size();i++)
{
if(validateRule(rules.at(i)) == 0)
controlON = false;
}

if (controlON == databases.at(index).controlON)
return "";
else
return "CHANGE" + std::to_string(controlON);
}

void DatabaseManager::connectToDatabaseTask(int deviceID)
{
if (findDatabaseIndex(deviceID) < 0)
{
Database newDB;
newDB.deviceID=deviceID;
newDB.sensorID= 0;
databases.push_back(newDB);
connectedDatabases.push(deviceID);
}
}

void DatabaseManager::connectToDatabase(int deviceID)
{
std::thread connectToDatabaseThread(&DatabaseManager::connectToDatabaseTask,this,deviceID);
connectToDatabaseThread.detach();
}

void DatabaseManager::addTemperatureData(int deviceID, int data)
{
Data entry;
entry.data = data;
entry.time = getTime();

int i = findDatabaseIndex(deviceID);
if ( i >= 0)
{
databases.at(i).temperatureData.push_back(entry);
}
}

void DatabaseManager::addMotionChange(int deviceID, int data)
{
Data entry;
entry.data = data;
entry.time = getTime();

int i = findDatabaseIndex(deviceID);
if ( i >= 0)
{
databases.at(i).motionChanges.push_back(entry);
}
}

void DatabaseManager::changeSensor(int deviceID, int newSensorID)
{
int i = findDatabaseIndex(deviceID);
if( i>=0)
{
databases.at(i).sensorID = newSensorID;
}
return;
}

int DatabaseManager::getSensor(int deviceID)
{
int i = findDatabaseIndex(deviceID);
if (i >= 0)
{
return databases.at(i).sensorID;
}
}

std::string getData(std::vector<Data> dataHistory, int number)
{
std::string output;
int j = 1;
for(std::vector<Data>::reverse_iterator it = dataHistory.rbegin(), 
itEnd = dataHistory.rend();
((it!=itEnd) || (j < number + 1));
++it, j++)
{
struct tm time = it->time;
int data = it->data;
char buffer[100];
strftime(buffer,100,"%d-%m-%Y %I:%M:%S", &time);
std::string timeString(buffer);
output += timeString + ":" + std::to_string(it->data) + "\n";	
}
return output;

}
int DatabaseManager::getTemperatureCurrent(int deviceID)
{
int i = findDatabaseIndex(deviceID);
if (i >= 0)
{
return databases.at(i).temperatureData.back().data;
}	
}

int DatabaseManager::getMotionChangesCurrent(int deviceID)
{
int i = findDatabaseIndex(deviceID);
if (i >= 0)
{
return databases.at(i).motionChanges.back().data;
}	
}

std::string DatabaseManager::getTemperatureRecent(int deviceID, int number)
{
std::string output= "Temperature Data \n";
int i = findDatabaseIndex(deviceID);
if ( i >= 0)
{
std::vector<Data> dataHistory = databases.at(i).temperatureData;
output+=getData(dataHistory, number);
}
return output;
}

std::string DatabaseManager::getMotionChangesRecent(int deviceID, int number)
{
std::string output= "Motion Changes\n";
int i = findDatabaseIndex(deviceID);
std::vector<Data> dataHistory = databases.at(i).motionChanges;
if ( i >= 0)
{
output+=getData(dataHistory, number);
}
return output;
}

void DatabaseManager::addRule(int deviceID, int attribute, int comparison, int value)
{
Rule rule;
rule.deviceID = deviceID;
rule.value = value;
rule.comparison = comparison;
rule.attribute = attribute;
rules.push_back(rule);
}

void DatabaseManager::removeRule(int deviceID, int attribute, int comparison, int value)
{
for(std::vector<Rule>::iterator it = rules.begin(); it<rules.end(); it++)
{
if(it->deviceID == deviceID &&
   it->value == value && 
   it->comparison == comparison && 
   it->attribute == attribute)
{
	rules.erase(it);
}

}
}

std::string DatabaseManager::getRules()
{
std::string output = "";
for(std::vector<Rule>::iterator it = rules.begin(); it<rules.end(); it++)
{
output+= "Device ID: " + std::to_string(it->deviceID);
output+= ", Attribute: " + std::to_string(it->attribute);
output+= ", Comparison: " + std::to_string(it->comparison);
output+= ", Value: " + std::to_string(it->value);
output+= "\n";
}
return output;
}

void DatabaseManager::setInterval(int deviceID, int newInterval)
{
int i = findDatabaseIndex(deviceID);
if (i>=0)
databases.at(i).interval = newInterval;
return;
}

int DatabaseManager::getInterval(int deviceID)
{
int i = findDatabaseIndex(deviceID);
if (i>=0)
return databases.at(i).interval;
return 0;
} 

bool DatabaseManager::empty()
{
return connectedDatabases.empty();
}

int DatabaseManager::front()
{
return connectedDatabases.front();
}

void DatabaseManager::pop()
{
connectedDatabases.pop();
return;
}

void DatabaseManager::printDatabase(int deviceID)
{
int i = findDatabaseIndex(deviceID);
if (i >= 0)
{
std::cout<<"Device: "<<databases.at(i).deviceID<<std::endl;
std::cout<<"Sensor: "<<databases.at(i).sensorID<<std::endl;
std::cout<<this->getTemperatureRecent(deviceID, 0)<<std::endl;
std::cout<<this->getMotionChangesRecent(deviceID, 0)<<std::endl;
}
std::cout<<std::endl<<"Rules"<<std::endl;
std::cout<<getRules()<<std::endl;
}
*/
