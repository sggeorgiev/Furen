#pragma once

#include <string>
#include "Server.h"

class Daemon {
private:
	std::string filename_;
	ServerPtr server_;
public:
	Daemon(std::string fileName, ServerPtr server);
	~Daemon();
	
public:
	void start();
	void stop();
	void restart();
	
private:
	void daemonize();
	void run();
};