#pragma once

#include <string>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

namespace Log {

enum Severity {
	TRACE,
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL
};

class Message {
public:
	Message();
	Message(Severity severity);
	Message(Severity severity, const boost::posix_time::ptime& time, unsigned long processId, unsigned long threadId, const std::string& text);
	~Message();
	
public:
	template<class T>
	Message& operator<<(const T& value) {
		std::stringstream stream;
		stream << value;
		text_ += stream.str();
		return *this;
	}
	
	Severity getSeverity() const;
	const boost::posix_time::ptime& getTime() const;
	unsigned long getProcessId() const;
	unsigned long getThreadId() const;
	const std::string& getText() const;
	
	void setSeverity(Severity severity);
	void setTime(const boost::posix_time::ptime& time);
	void setProcessId(unsigned long processId);
	void setThreadId(unsigned long threadId);
	void setText(const std::string& text);
	
private:
	unsigned long threadIdToLong(const boost::thread::id& threadId);
	
private:
	Severity severity_;
	boost::posix_time::ptime time_;
	unsigned long processId_;
	unsigned long threadId_;
	std::string text_;
};

};