#include "headers/logger.h"

void Logger::logEvent(LogEvents event, std::string logstr)
{
	std::string sEvent {};

	switch (event)
	{
		case LogEvents::INFO:
			sEvent = "[INFO] :: ";
			break;
		case LogEvents::DEBUG:
			sEvent = "[DEBUG] :: ";
			break;
		case LogEvents::MOVEMENT:
			sEvent = "[MOVEMENT] :: ";
			break;
		case LogEvents::PREDICTION:
			sEvent = "[PREDICTION] :: ";
			break;
		case LogEvents::FALLBACK:
			sEvent = "[FALLBACK] :: ";
			break;
		default:
			break;
	}

	std::ofstream logFile {"log.txt", std::ios_base::app};
	if (logFile.is_open())
	{
		logFile << sEvent << logstr << std::endl;
		logFile.close();
	}
}
