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

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <Log.h>
#include "Server.h"

class TestServer: public Transport::Server {
public:
	TestServer(Transport::IOServeice& ioServeice, const Transport::Endpoint& endpoint): Transport::Server(ioServeice, endpoint) {
	}
	
	void handleReceiveMessage(const Transport::SessionPtr& session, const Transport::MessagePtr& message, const Utilities::ErrorPtr& error) {
		if(!error) {
			std::string data(message->getBody(), message->getBodyLength());
			std::cout << "Received:" << data << std::endl;
			
			session->write(boost::bind(&TestServer::handleWrite, this, _1), message);
		}
	}
	
	void handleConnectionClose(const Transport::SessionPtr& session, const Utilities::ErrorPtr& error) {
		std::cout << "connection close: " << error;
	}
	
	void handleWrite(const Utilities::ErrorPtr& error) {
		if(error)
			std::cout << "Write fail!" << std::endl;
		else 
			std::cout << "Write complete!" <<std::endl;
	}
};

int main () {
	Log::init("./", "server_log", "trace", "%Time% %Severity% %ProcessId% %ThreadId% %Text%", 104857600);
	
	Transport::IOServeice ioServeice;
	Transport::Endpoint endpoint(boost::asio::ip::tcp::v4(), 8090);
	boost::shared_ptr<TestServer> testServer(new TestServer(ioServeice, endpoint));
	testServer->start();
	
	ioServeice.run();
	return 0;
}