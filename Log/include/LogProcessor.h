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

#pragma once

#include <Definitions.h>
#include <MessageProcessor.h>
#include <Pipe.h>
#include <File.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace Log {

class LogProcessor: public boost::enable_shared_from_this<LogProcessor> {
public:
	typedef boost::shared_ptr<LogProcessor> LogProcessorPtr;
	
public:
	virtual ~LogProcessor();
	
public: 
	static LogProcessor& instance();
	
public:
	void init(const std::string& directoryName, const std::string& fileName, const std::string& level, const std::string& format, unsigned long rotationSize);
	LogProcessor& operator+=(const Message& message);
	
private:
	LogProcessor();
	void writeToPipe(const std::string& message);
	void writeToFile(const std::string& message);
	void readFromPipe();
		
	Severity getLogLevel(const std::string& level);
	
private:
	static LogProcessorPtr instance_;
	static boost::mutex mutex_;
	
	MessageProcessor messageProcessor_;
	Pipe pipe_;
	FilePtr file_;
	boost::thread thread_;
	
	std::string directoryName_;
	std::string fileName_;
	std::string level_;
	std::string format_;
	unsigned long rotationSize_;
};

#define LOG(severity) (Log::LogProcessor::instance()) += Log::Message(severity)

};