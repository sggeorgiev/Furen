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

#include "Service.h"

class Daemon {
public:
	Daemon(const ServicePtr& server);
	~Daemon();
	
public:
	void start();
	void stop();
	void restart();
	bool loadAndVlidateConfigFile(const std::string& configFile);
	void main(int argc, char **argv);
private:
	void daemonize();
	void run();
	
private:
	std::string name_;
	std::string pidFileName_;
	std::string directoryName_;
	std::string fileName_;
	std::string level_;
	std::string format_;
	unsigned long rotationSize_;
	
	ServicePtr service_;
};

typedef boost::shared_ptr<Daemon> DaemonPtr;