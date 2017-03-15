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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

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

void LogProcessor::init(const std::string& configFileName) {
	boost::property_tree::ptree configurationTree;
	boost::property_tree::read_xml(configFileName, configurationTree);
	
	//Log configuration
	fileName_ = configurationTree.get<std::string>("configuration.log.file_name");
	level_ = configurationTree.get<std::string>("configuration.log.level");
	format_ = configurationTree.get<std::string>("configuration.log.format");
	rotationSize_ = configurationTree.get<unsigned long>("configuration.log.rotation_size");
	timeBasedRotationHour_ = configurationTree.get<unsigned char>("configuration.log.time_based_rotation.hour");
	timeBasedRotationMinute_ = configurationTree.get<unsigned char>("configuration.log.time_based_rotation.minute");
	timeBasedRotationSecond_ = configurationTree.get<unsigned char>("configuration.log.time_based_rotation.second");
	
	thread_ = boost::thread(boost::bind(&LogProcessor::readFromPipe, shared_from_this()));
}

LogProcessor& LogProcessor::operator+=(const Message& message) {
	std::string textMessage = messageProcessor_.formatMessage(format_, message);
	writeToPipe(textMessage);
	
	return *this;
}

void LogProcessor::writeToPipe(const std::string& message) {
	pipe_.write(message);
}

void LogProcessor::writeToFile(const std::string& message) {
	std::cout << "M: " << message << std::endl;
}

void LogProcessor::readFromPipe() {
	while(1) {
		std::string message = pipe_.read();
		writeToFile(message);
	}
}

Severity LogProcessor::getLogLevel(const std::string& level) {
	if(level == "trace")
		return TRACE;
	else if(level == "debug")
		return DEBUG;
	else if(level == "info")
		return INFO;
	else if(level == "warning")
		return WARNING;
	else if(level == "error")
		return ERROR;
	else if(level == "fatal")
		return FATAL;
	
	return TRACE;
}

};
