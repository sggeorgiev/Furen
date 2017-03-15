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

#include "include/Daemon.h"
#include <iostream>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace pt = boost::property_tree;

Daemon::Daemon(const std::string& configFileName, const ServerPtr& server): server_(server) {
	pt::read_xml(configFileName, configurationTree_);
	
	//Main configuration
	name_ = configurationTree_.get<std::string>("configuration.main.name");
	pidFileName_ = configurationTree_.get<std::string>("configuration.main.pid_file_name");
}

Daemon::~Daemon() {
}

void Daemon::start() {
	//BOOST_LOG_SEV(log_, boost::log::trivial::info) << "START";
	
	//Check for a pidfile to see if the daemon already runs
	pid_t pid = 0;
	if(fs::exists(pidFileName_)) {
		FILE* fp = fopen(pidFileName_.c_str(), "r");
		fscanf(fp, "%d", &pid);
		fclose(fp);
		std::cout << name_ << " is already running. pid: " << pid << std::endl;
		return;
	}
	// Start the daemon
	daemonize();
	run();
}

void Daemon::stop() {
	//BOOST_LOG_SEV(log_, boost::log::trivial::info) << "STOP";
	
	// Get the pid from the pidfile
	if(!fs::exists(pidFileName_)) {
		std::cout << name_ << " is not running." << std::endl;
		return;
	}
	
	pid_t pid  = 0;
	FILE* fp = fopen(pidFileName_.c_str(), "r");
	fscanf(fp, "%d", &pid);
	fclose(fp);
	
	// Try killing the daemon process
	for(int i=0; i<10; i++) {
		int result = kill(pid, SIGTERM);
		if(result == 0) { //success
			if(fs::exists(pidFileName_))
				fs::remove(pidFileName_);
			return;
		}
		sleep(1);
	}
}

void Daemon::restart() {
	//BOOST_LOG_SEV(log_, boost::log::trivial::info) << "RESTART";
	
	stop();
	start();
}

void Daemon::daemonize() {
	pid_t pid = 0;
	if(pid = fork())
	{
		if(pid > 0)
		{
		// We're in the parent process and need to exit.
			exit(0);
		}
		else
		{
			//BOOST_LOG_SEV(log_, boost::log::trivial::error) << "First fork failed!";			
			exit(1);
		}
	}
	// Make the process a new session leader. This detaches it from the
	// terminal.
	setsid();

	// A process inherits its working directory from its parent. This could be
	// on a mounted filesystem, which means that the running daemon would
	// prevent this filesystem from being unmounted. Changing to the root
	// directory avoids this problem.
	chdir("/");

	// The file mode creation mask is also inherited from the parent process.
	// We don't want to restrict the permissions on files created by the
	// daemon, so the mask is cleared.
	umask(0);

	// A second fork ensures the process cannot acquire a controlling terminal.
	if(pid = fork())
	{
		if(pid > 0)
		{
			exit(0);
		}
		else
		{
			//BOOST_LOG_SEV(log_, boost::log::trivial::error) << "Second fork failed!";
			exit(1);
		}
	}

	// Close the standard streams. This decouples the daemon from the terminal
	// that started it.
	close(0);
	close(1);
	close(2);

	// We don't want the daemon to have any standard input.
	if(open("/dev/null", O_RDONLY) < 0)
	{
		//BOOST_LOG_SEV(log_, boost::log::trivial::error) << "Unable to open /dev/null!";
		exit(1);
	}	
	
	pid = getpid();
	FILE* fp = fopen(pidFileName_.c_str(), "w");
	fprintf(fp, "%d", pid);
	fclose(fp);
}

void Daemon::run() {
	if(server_)
		server_->run();
	
	if(fs::exists(pidFileName_))
		fs::remove(pidFileName_);
}

void Daemon::main(int argc, char **argv) {
	po::options_description description("Options");
	description.add_options()
		("help", "Show help message.")
		("start", "Start daemon.")
		("stop", "Stop daemon.")
		("restart", "Restart daemon.");
		
	po::variables_map variablesMap;
	po::store(po::parse_command_line(argc, argv, description), variablesMap);
	po::notify(variablesMap);
	
	if(variablesMap.count("start")) {
		start();
	}
	else if(variablesMap.count("stop")) {
		stop();
	}
	else if(variablesMap.count("restart")) {
		restart();
	}
	else {
		std::cout << description << std::endl;
	}
}