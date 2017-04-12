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
#include "Message.h"
#include "Error.h"
#include "ErrorCode.h"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace Transport {

class Session: public boost::enable_shared_from_this<Session> {
private:
	const static int HEARTBEAT_INTERVAL = 10;
public:
	Session(IOServeice& ioServeice);
	virtual ~Session();
	
public:
	void start(const ReadCallback& readCallback);
	Socket& getSocket();
	
public:
	void write(const WriteCallback& callback, const MessagePtr& message);
	
private:
	void handleReadMessageHeader(const boost::system::error_code& error);
	void handleReadMessageBody(const boost::system::error_code& error);
	void handleWrite(const WriteCallback& callback, const boost::system::error_code& error);
	void heartbeatHandle(const boost::system::error_code& errorCode);
	void heartbeatWriteHandle(const Utilities::ErrorPtr& error);
	void resetHeartbeatTimer();
	
private:
	Socket socket_; 
	ReadCallback readCallback_;
	MessagePtr message_;
	MessageItemQueue messageItemQueue_;
	Timer heartbeatTimer_;
};

typedef boost::shared_ptr<Session> SessionPtr;

};