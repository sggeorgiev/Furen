#pragma once

#include "Message.h"
#include <string>

namespace Log {

class MessageProcessor {
public:
	MessageProcessor();
	~MessageProcessor();
	
public:
	std::string formatMessage(const std::string& format, const Message& message);
	
private:
	std::string convertSeverityToString(Severity severity);
	std::string convertTimeToString(const boost::posix_time::ptime& time);
};

}