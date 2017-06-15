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

#include "include/Message.h"
#include <string.h>
#include <stdlib.h>

namespace Transport {

const static std::string HEARTBEAT_MESSAGE = "PING";
	
Message::Message() {
}

Message::~Message() {
}

void Message::setData(const std::string& data) {
	if(data.length() > Message::MAX_BODY_SIZE) {
		//TODO: error check
	}
	
	bodyLength_ = data.length();
	strncpy(data_ + Message::HEADER_SIZE, data.c_str(), data.length());
	encodeHeader();
}

char* Message::getData() {
	return data_;
}

char* Message::getBody() {
	return data_ + Message::HEADER_SIZE;
}

unsigned int Message::getBodyLength() const  {
	return bodyLength_;
}

unsigned int Message::getLength() const
{
	return Message::HEADER_SIZE + bodyLength_;
}
  
bool Message::decodeHeader() {
	char header[Message::HEADER_SIZE + 1] = "";
	strncat(header, data_, Message::HEADER_SIZE);
	bodyLength_ = atoi(header);
	if (bodyLength_ > Message::MAX_BODY_SIZE) {
		bodyLength_ = 0;
		return false;
	}
	return true;
}

void Message::encodeHeader() {
	char header[Message::HEADER_SIZE + 1] = "";
	sprintf(header, "%5d", bodyLength_);
	memcpy(data_, header, Message::HEADER_SIZE);
}

void Message::createHeartbeatMessage() {
	setData(HEARTBEAT_MESSAGE);
}

bool Message::isHeartbeat() {
	if(strncmp(data_ + Message::HEADER_SIZE, HEARTBEAT_MESSAGE.c_str(), HEARTBEAT_MESSAGE.length()) == 0)
		return true;
	return false;
}

};