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
	bool isConnected;
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

class DatabaseManager
{
	private:
		std::vector<Device> database;
	public:
		const int NONE = 0;
		const int TEMP = 1;
		const int MOTION = 2;
		const int OUTLET = 3;
		const int GREATERTHAN = 0;
		const int LESSTHAN = 1;
		
		DatabaseManager();
		void setSensorID(int deviceID, int sensorID);
		void setIsConnected(int deviceID, bool connected);
		void setTemperatureInterval(int deviceID, int interval);
		void setTemperatureCurrent(int deviceID, int temperature);
		void setMotionDelay(int deviceID, int delay);
		void setMotionStatus(int deviceID, bool motion);
		void setOutletStatus(int deviceID, bool powerOn);
		void setOutletRule(int outlet_deviceID, int sensor_deviceID, int sensor, int value, int comparator);
		
		int getSensorID(int deviceID);
		bool getIsConnected(int deviceID);
		int getTemperatureInterval(int deviceID);
		int getTemperatureCurrent(int deviceID);
		std::string getTemperatureCurrentString(int deviceID);
		int getMotionDelay(int deviceID);
		bool getMotionStatus(int deviceID);
		std::string getMotionStatusString(int deviceID);
		bool getOutletStatus(int deviceID);
		int getOutletRule_deviceID(int deviceID);
		int getOutletRule_sensor(int deviceID);
		int getOutletRule_value(int deviceID);
		int getOutletRule_comparator(int deviceID);

		bool checkRules();

		void printDatabase();
		void saveDatabase();
		void loadDatabase();
};

#endif
