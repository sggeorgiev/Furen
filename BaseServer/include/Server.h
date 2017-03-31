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

#include "Definitions.h"
#include "Message.h"
#include "Session.h"
#include "SessionManager.h"

namespace BaseServer {

class Server: public boost::enable_shared_from_this<Server> {
public:
	Server(IOServeice& ioServeice, const Endpoint& endpoint);
	~Server();
	
public:
	void start();
	
private:
	void accept();
	void handleAccept(const SessionPtr& session, const boost::system::error_code& error);
	
private:
	IOServeice& ioServeice_;
	Endpoint endpoint_;
	Acceptor acceptor_;
	SessionManagerPtr sessionManager_;
};

typedef boost::shared_ptr<Server> ServerPtr;

}