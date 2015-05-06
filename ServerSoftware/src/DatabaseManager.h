#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <queue>
#include <ctime>
#include <string>

#define NUMBER_OF_DEVICES 3

//Settings and data for each MCU device
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
};

class DatabaseManager
{
	private:
		//List of all MCU's data and settings
		std::vector<Device> database;
		//Last time temperature was read
		time_t lastRead;
	public:
		const int NONE = 0;
		const int TEMPERATURE = 1;
		const int MOTION = 2;
		const int OUTLET = 3;
		
		DatabaseManager();
		void setSensorID(int deviceID, int sensorID);
		void setIsConnected(int deviceID, bool connected);
		void setTemperatureInterval(int deviceID, int interval);
		void setTemperatureCurrent(int deviceID, int temperature);
		void setMotionDelay(int deviceID, int delay);
		void setMotionStatus(int deviceID, bool motion);
		void setOutletStatus(int deviceID, bool powerOn);
		
		int getSensorID(int deviceID);
		bool getIsConnected(int deviceID);
		int getTemperatureInterval(int deviceID);
		int getTemperatureCurrent(int deviceID);
		std::string getTemperatureTime(int deviceID);
		int getMotionDelay(int deviceID);
		bool getMotionStatus(int deviceID);
		std::string getMotionStatusTime(int deviceID);
		bool getOutletStatus(int deviceID);

		int checkTemperature();

		void printDatabase();
		void saveDatabase();
		void loadDatabase();
};

#endif
