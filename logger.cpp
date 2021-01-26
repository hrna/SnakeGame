#include "logger.h"

void Logger::logEvent(std::string str)
{	
	std::ofstream logFile {"log.txt", std::ios_base::app};
	if (logFile.is_open())
	{
		logFile << str << std::endl;
		logFile.close();
	}
}
