#pragma once

#include <Definitions.h>
#include <MessageProcessor.h>
#include <Pipe.h>

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
	void init(const std::string& configFileName);
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
	boost::thread thread_;
	
	std::string fileName_;
	std::string level_;
	std::string format_;
	unsigned long rotationSize_;
	unsigned char timeBasedRotationHour_;
	unsigned char timeBasedRotationMinute_;
	unsigned char timeBasedRotationSecond_;
};

static inline void init(const std::string& configFileName) {
	LogProcessor::instance().init(configFileName);
}

#define LOG(severity) (Log::LogProcessor::instance()) += Log::Message(severity)

};