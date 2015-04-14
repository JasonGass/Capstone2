#ifndef DATAFILE_H
#define DATAFILE_H

#include <string>
#include <queue>
#include <fstream>
#include <iostream>

class DataFile
{
	private:
		std::fstream file;
		std::string filePath; 
	public:
		DataFile(std::string path);
		void addEntry(std::string entry);
		std::string getPreviousEntries(int numberOfEntries);
};
#endif
