#include <iostream>
#include <fstream>
#include "DataFile.h"

int main()
{
	DataFile data("/home/server/Capstone2/ServerSoftware/DATA/data.txt");
	data.addEntry("test");
	data.getPreviousEntries(7);
	std::cout<<std::endl;
}
