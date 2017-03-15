/**
 * Copyright (C) 2017 Sergey Georgiev (sgeorgiev@cpp-examples.com)
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * 
 */

#include "MessageProcessor.h"
#include <boost/lexical_cast.hpp>

namespace Log {

MessageProcessor::MessageProcessor() {
	
}

MessageProcessor::~MessageProcessor() {
	
}

std::string MessageProcessor::formatMessage(const std::string& format, const Message& message) {
	std::string textMessage = format;
	
	std::string severity = convertSeverityToString(message.getSeverity());
	textMessage.replace(textMessage.find("%Severity%"), std::string("%Severity%").length(), severity);
	
	std::string time = convertTimeToString(message.getTime());
	textMessage.replace(textMessage.find("%Time%"), std::string("%Time%").length(), time);
	
	std::string processId = boost::lexical_cast<std::string>(message.getProcessId());
	textMessage.replace(textMessage.find("%ProcessId%"), std::string("%ProcessId%").length(), processId);
	
	std::string threadId = boost::lexical_cast<std::string>(message.getThreadId());
	textMessage.replace(textMessage.find("%ThreadId%"), std::string("%ThreadId%").length(), threadId);
	
	std::string text = message.getText();
	textMessage.replace(textMessage.find("%Text%"), std::string("%Text%").length(), text);
	
	return textMessage;
}

std::string MessageProcessor::convertSeverityToString(Severity severity) {
	if(severity == TRACE)
		return "TRACE";
	if(severity == DEBUG)
		return "DEBUG";
	if(severity == INFO)
		return "INFO";
	if(severity == WARNING)
		return "WARNING";
	if(severity == ERROR)
		return "ERROR";
	if(severity == FATAL)
		return "FATAL";
	
	return "TRACE";
}

std::string MessageProcessor::convertTimeToString(const boost::posix_time::ptime& time) {
	return to_simple_string(time);
}

}