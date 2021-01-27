#ifndef _LOGGER_
#define _LOGGER_

#include <iostream>
#include <fstream>

enum class LogEvents {INFO, DEBUG, MOVEMENT, PREDICTION, FALLBACK};

class Logger
{
private:

public:
	Logger();
	~Logger();

	void logEvent(LogEvents event, std::string);

};

#endif
