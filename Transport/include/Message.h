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

#include <boost/shared_ptr.hpp>

namespace Transport {
	
class Message {
public:
	const static unsigned int HEADER_SIZE = 5;
	const static unsigned int MAX_BODY_SIZE = 99999;
	
public:
	Message();
	~Message();
	
	void setData(const std::string& data);
	
	char* getData();
	char* getBody();
	unsigned int getLength() const;
	unsigned int getBodyLength() const ;
	
	bool decodeHeader();
	
private:
	void encodeHeader();
	
private:
	char data_[HEADER_SIZE + MAX_BODY_SIZE];
	unsigned int bodyLength_;
};

typedef boost::shared_ptr<Message> MessagePtr;

};