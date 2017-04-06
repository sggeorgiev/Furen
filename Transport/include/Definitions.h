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
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <deque>
#include <Error.h>
#include "Message.h"

namespace Transport {

typedef boost::asio::io_service IOServeice;
typedef boost::asio::ip::tcp::endpoint Endpoint;
typedef boost::asio::ip::tcp::resolver::iterator EndpointIterator;
typedef boost::asio::ip::tcp::acceptor Acceptor;

typedef boost::asio::ip::tcp::socket Socket;
typedef unsigned long SessionId;

typedef boost::function<void (const Transport::MessagePtr&, const Utilities::ErrorPtr&)> ReadCallback;
typedef boost::function<void (const Utilities::ErrorPtr&)> WriteCallback;

struct MessageItem {
	MessagePtr message;
	WriteCallback callback;
};

typedef boost::shared_ptr<MessageItem> MessageItemPtr;
typedef std::deque<MessageItemPtr> MessageItemQueue;

};