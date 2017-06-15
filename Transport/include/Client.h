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

#include "Definitions.h"
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace Transport {
	
class Client: public boost::enable_shared_from_this<Client> {
public:
	Client(IOServeice& ioServeice, const EndpointIterator& endpointIterator);
	virtual ~Client();
	
public:
	void start(const ReadCallback& readCallback);
	void write(const WriteCallback& writeCallback, const MessagePtr& message);
	
private:
	void handleConnect(const boost::system::error_code& error);
	void handleReadMessageHeader(const boost::system::error_code& error);
	void handleReadMessageBody(const boost::system::error_code& error);
	void handleWrite(const WriteCallback& callback, const boost::system::error_code& errorCode);
	void handleHeartbeatWrite(const Utilities::ErrorPtr& error);
private:
	IOServeice& ioServeice_;
	EndpointIterator endpointIterator_;
	ReadCallback readCallback_;
	
	Socket socket_; 
	MessagePtr message_;
	MessageItemQueue messageItemQueue_;

};

typedef boost::shared_ptr<Client> ClientPtr;

};