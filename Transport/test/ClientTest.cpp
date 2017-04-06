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

#include "Client.h"
#include <Log.h>

static void handleRead(const Transport::MessagePtr& message, const Utilities::ErrorPtr& error) {
	if(!error) {
		std::string data(message->getBody(), message->getBodyLength());
		std::cout << "Received: "<< data << std::endl;
	}
	else {
		std::cout << error << std::endl;
	}
}

static void handleWrite(const Utilities::ErrorPtr& error) {
	if(!error) {
		std::cout <<"Write complete!" << std::endl;		
	}
	else {
		std::cout << "Write fail!" << std::endl;
	}
}

int main() {
	Log::init("./", "server_log", "trace", "%Time% %Severity% %ProcessId% %ThreadId% %Text%", 104857600);
	
	Transport::IOServeice ioServeice;
	boost::asio::ip::tcp::resolver resolver(ioServeice);
	boost::asio::ip::tcp::resolver::query query("127.0.0.1", "8090");
	Transport::EndpointIterator endpointIterator = resolver.resolve(query);
	Transport::ClientPtr client(new Transport::Client(ioServeice, endpointIterator));
	
	client->start(&handleRead);
	
	Transport::MessagePtr message(new Transport::Message);
	message->setData("test test test");
	client->write(&handleWrite, message);
	
	ioServeice.run();
	
	return 0;
}