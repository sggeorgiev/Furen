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

#include "include/Server.h"
#include "Log.h"

namespace BaseServer {
	
Server::Server(IOServeice& ioServeice, const Endpoint& endpoint, const SessionManagerPtr& sessionManager): ioServeice_(ioServeice), endpoint_(endpoint), acceptor_(ioServeice, endpoint), sessionManager_(new SessionManager) {
}

Server::~Server(){
}

void Server::start() {
	accept();
}

void Server::accept() {
	SessionPtr session(new Session(ioServeice_));
	acceptor_.async_accept(session->getSocket(), boost::bind(&Server::handleAccept, this, session, boost::asio::placeholders::error));
}

void Server::handleAccept(const SessionPtr& session, const boost::system::error_code& errorCode) {
	if (!errorCode) {
		sessionManager_->addSession(session);
		session->start();
	}
	else {
		LOG(Log::ERROR) << "accept fail: " << errorCode;
	}

	accept();
}

}