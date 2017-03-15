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

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Server.h"

class Daemon {
private:
	std::string name_;
	std::string pidFileName_;
	boost::property_tree::ptree configurationTree_;
	
	ServerPtr server_;
public:
	Daemon(const std::string& configFileName, const ServerPtr& server);
	~Daemon();
	
public:
	void start();
	void stop();
	void restart();
	void main(int argc, char **argv);
private:
	void daemonize();
	void run();
};

typedef boost::shared_ptr<Daemon> DaemonPtr;