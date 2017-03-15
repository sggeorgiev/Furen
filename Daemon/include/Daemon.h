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