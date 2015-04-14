#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include "DataFile.h"

DataFile::DataFile(std::string path):filePath(path)
{
}

void DataFile::addEntry(std::string entry)
{
	file.open(filePath.c_str(), std::fstream::out | std::fstream::app);
	std::time_t currentTime = std::time(0);
	struct tm * localTime = std::localtime(&currentTime);

	file << localTime->tm_year+1900<<"-"
		 <<localTime->tm_mon+1<<"-"
		 <<localTime->tm_mday<<","
		 <<localTime->tm_hour<<":"
		 <<localTime->tm_min<<":"
		 <<localTime->tm_sec<<","
		 <<entry<<std::endl;
	file.close();
}

std::string DataFile::getPreviousEntries(int numberOfEntries)
{
	file.open(filePath.c_str(), std::fstream::in);
	std::string entry;
	std::queue<std::string> entries;
	std::string entriesToReturn = "";
	int i = 0;

	while(std::getline(file,entry))
	{
		entries.push(entry);
		if(numberOfEntries != 0 && entries.size() > numberOfEntries)
		{
			entries.pop();
		}
	}

	while(!entries.empty())
	{
		entriesToReturn += entries.front();
		entriesToReturn += "\n";
		entries.pop();
	}	

	file.close();
	return entriesToReturn;
}

