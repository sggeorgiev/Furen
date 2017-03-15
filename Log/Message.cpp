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

#include "Message.h"
#include <boost/thread.hpp>

namespace Log {

Message::Message() {
	severity_ = TRACE;
	processId_ = 0;
	threadId_ = 0;
}

Message::Message(Severity severity) {
	severity_ = severity;
	time_ = boost::posix_time::microsec_clock::local_time();
	processId_ = getpid();
	threadId_ = threadIdToLong(boost::this_thread::get_id());
	text_ = "";
}

Message::Message(Severity severity, const boost::posix_time::ptime& time, unsigned long processId, unsigned long threadId, const std::string& text) {
	severity_ = severity;
	time_ = time;
	processId_ = processId;
	threadId_ = threadId;
	text_ = text;
}

Message::~Message() {
}

Severity Message::getSeverity() const {
	return severity_;
}

const boost::posix_time::ptime& Message::getTime() const {
	return time_;
}

unsigned long Message::getProcessId() const {
	return processId_;
}

unsigned long Message::getThreadId() const {
	return threadId_;
}

const std::string& Message::getText() const {
	return text_;
}

void Message::setSeverity(Severity severity) {
	severity_ = severity;
}

void Message::setTime(const boost::posix_time::ptime& time) {
	time_ = time;
}

void Message::setProcessId(unsigned long processId) {
	processId_ = processId;
}

void Message::setThreadId(unsigned long threadId) {
	threadId_ = threadId;
}

void Message::setText(const std::string& text) {
	text_ = text;
}

unsigned long Message::threadIdToLong(const boost::thread::id& threadId) {
	unsigned long value;
	std::stringstream ss;
	ss << threadId;
	ss >> value;
	return value;
}

}