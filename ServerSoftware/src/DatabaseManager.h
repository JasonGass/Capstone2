#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <queue>
#include <ctime>
#include <string>

struct TemperatureData
{
	int temperature;
	struct tm_time;
};
struct Device
{
	int deviceID;
	int sensorID;
	int temperature_interval;
	int temperature_current;
	struct tm temperature_timeSinceReading;
	int motion_delay;
	bool motion_status;
	struct tm motion_timeSinceChange;
	bool outlet_on;
	int outlet_rule_deviceID;
	int outlet_rule_value;
	int outlet_rule_sensor;
	int outlet_rule_comparator;
};
struct Database
{
	Device device1;
	Device device2;
	Device device3;
};
struct Data
{
	struct tm time;
	int data;
};

struct Database
{
	std::vector<Data> temperatureData;
	std::vector<Data> motionChanges;
	int deviceID;
	int sensorID;
	int interval;
	bool controlON;
};
struct Rule
{
	int deviceID;
	int value;
	int comparison;
	int attribute;
};

class DatabaseManager
{
	private:
		std::vector<Database> databases;
		std::queue<int> connectedDatabases;
		int findDatabaseIndex(int deviceID);
		std::vector<Rule> rules;
	public:
		const int NONE = 0;
		const int TEMP = 1;
		const int MOTION = 2;
		const int OUTLET = 3;
		const int GREATERTHAN = 0;
		const int LESSTHAN = 1;
		
		DatabaseManager();
		int findControlIndex();
		void connectToDatabase(int deviceID);
		void connectToDatabaseTask(int deviceID);
		void addTemperatureData(int deviceID, int data);
		void addMotionChange(int deviceID, int data);
		void changeSensor(int deviceID, int newSensorID);
		int getSensor(int deviceID);
		std::string getTemperatureRecent(int deviceID, int number);
		std::string getMotionChangesRecent(int deviceID, int number);
		int getTemperatureCurrent(int deviceID);
		int getMotionChangesCurrent(int deviceID);
		void addRule(int deviceID, int value, int comparison, int attribute);
		void removeRule(int deviceID, int value, int comparison, int attribute);
		std::string getRules();
		void setInterval(int deviceID, int newInterval);
		int getInterval(int deviceID);
		std::string checkForUpdates();
		int validateRule(Rule rule);
		bool empty();
		int front();
		void pop();
		void printDatabase(int deviceID);

};

#endif
