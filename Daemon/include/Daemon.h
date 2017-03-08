#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

#include "Server.h"

class Daemon {
private:
	std::string fileName_;
	ServerPtr server_;
public:
	Daemon(const std::string& fileName, const ServerPtr& server);
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