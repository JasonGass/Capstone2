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
	std::string temperature_timeSinceReading;
	int motion_delay;
	bool motion_status;
	std::string motion_timeSinceChange;
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
		time_t lastRead;
	public:
		const int NONE = 0;
		const int TEMPERATURE = 1;
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
		std::string getTemperatureTime(int deviceID);
		int getMotionDelay(int deviceID);
		bool getMotionStatus(int deviceID);
		std::string getMotionStatusTime(int deviceID);
		bool getOutletStatus(int deviceID);
		int getOutletRule_deviceID(int deviceID);
		int getOutletRule_sensor(int deviceID);
		int getOutletRule_value(int deviceID);
		int getOutletRule_comparator(int deviceID);

		int checkRules();
		int checkTemperature();

		void printDatabase();
		void saveDatabase();
		void loadDatabase();
};

#endif
