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
#include <deque>

namespace BaseServer {

typedef boost::function<void (const MessagePtr&, const Utilities::ErrorPtr&)> ReadCallback;
typedef boost::function<void (const Utilities::ErrorPtr&)> WriteCallback;
	
struct MessageItem {
	MessagePtr message;
	WriteCallback callback;
};

class Session: public boost::enable_shared_from_this<Session> {
public:
	Session(IOServeice& ioServeice);
	virtual ~Session();
	
public:
	void start();
	Socket& getSocket();
	
public:
	void read(const ReadCallback& callback);
	void write(const WriteCallback& callback, const MessagePtr& message);
	
private:
	void handleRead(const MessagePtr& message, const Utilities::ErrorPtr& error);
	void handleReadMessageHeader(const ReadCallback& callback, const boost::system::error_code& error);
	void handleReadMessageBody(const ReadCallback& callback, const boost::system::error_code& error);
	void handleWrite(const WriteCallback& callback, const boost::system::error_code& error);
	
private:
	Socket socket_; 
	MessagePtr message_;
	
	typedef boost::shared_ptr<MessageItem> MessageItemPtr;
	typedef std::deque<MessageItemPtr> MessageItemQueue;
	MessageItemQueue messageItemQueue_;
};

typedef boost::shared_ptr<Session> SessionPtr;

};