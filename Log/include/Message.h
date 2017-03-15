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