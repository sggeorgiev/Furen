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

#include <LogProcessor.h>
#include <boost/algorithm/string.hpp>

namespace Log {

LogProcessor::LogProcessorPtr LogProcessor::instance_;
boost::mutex LogProcessor::mutex_;
	
LogProcessor::LogProcessor() {
}

LogProcessor::~LogProcessor() {
}

LogProcessor& LogProcessor::instance() {
	if(instance_.get() == 0) {
		boost::lock_guard<boost::mutex> guard(mutex_);
		if(instance_.get() == 0) {
			instance_.reset( new LogProcessor );
		}
	}
        
	return *instance_;
}

void LogProcessor::init(const std::string& directoryName, const std::string& fileName, const std::string& level, const std::string& format, unsigned long rotationSize) {
	directoryName_ = directoryName;
	fileName_ = fileName;
	level_ = level;
	format_ = format;
	rotationSize_ = rotationSize;
	
	thread_ = boost::thread(boost::bind(&LogProcessor::readFromPipe, shared_from_this()));
	file_ = FilePtr(new Log::File(directoryName_, fileName_, rotationSize_));
}

LogProcessor& LogProcessor::operator+=(const Message& message) {
	if(file_) {
		std::string textMessage = messageProcessor_.formatMessage(format_, message);
		writeToPipe(textMessage);
	}
	
	return *this;
}

void LogProcessor::writeToPipe(const std::string& message) {
	pipe_.write(message);
}

void LogProcessor::writeToFile(const std::string& message) {
	if(file_)
		file_->write(message);
}

void LogProcessor::readFromPipe() {
	while(1) {
		std::string message = pipe_.read();
		writeToFile(message);
	}
}

Severity LogProcessor::getLogLevel(const std::string& level) {
	if(boost::iequals(level, "trace"))
		return TRACE;
	else if(boost::iequals(level, "debug"))
		return DEBUG;
	else if(boost::iequals(level, "info"))
		return INFO;
	else if(boost::iequals(level, "warning"))
		return WARNING;
	else if(boost::iequals(level, "error"))
		return ERROR;
	else if(boost::iequals(level, "fatal"))
		return FATAL;
	
	return TRACE;
}

};
