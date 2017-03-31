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

#include "Daemon.h"
#include "Log.h"
#include <iostream>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace pt = boost::property_tree;

Daemon::Daemon(const ServicePtr& service): service_(service) {
}

Daemon::~Daemon() {
}

void Daemon::start() {
	//Check for a pidfile to see if the daemon already runs
	pid_t pid = 0;
	if(fs::exists(pidFileName_)) {
		FILE* fp = fopen(pidFileName_.c_str(), "r");
		if(fp == NULL) {
			std::cerr << "Cannot open file: " << pidFileName_ << " Error: " << strerror(errno);
			return;
		}
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
	// Get the pid from the pidfile
	if(!fs::exists(pidFileName_)) {
		std::cout << name_ << " is not running." << std::endl;
		return;
	}
	
	pid_t pid  = 0;
	FILE* fp = fopen(pidFileName_.c_str(), "r");
	if(fp == NULL) {
		std::cerr << "Cannot open file: " << pidFileName_ << " Error: " << strerror(errno);
		return;
	}
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
	stop();
	start();
}

void Daemon::daemonize() {
	pid_t pid = fork();
	if(pid != 0)
	{
		if(pid > 0)
		{
			// We're in the parent process and need to exit.
			exit(0);
		}
		else
		{
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
	pid = fork();
	if(pid != 0)
	{
		if(pid > 0)
		{
			// We're in the parent process and need to exit.
			exit(0);
		}
		else
		{
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
		LOG(Log::ERROR) << "Unable to open /dev/null!";
		exit(1);
	}	
	
	//open log
	Log::init(directoryName_, fileName_, level_, format_, rotationSize_);
	
	pid = getpid();
	FILE* fp = fopen(pidFileName_.c_str(), "w");
	if(fp == NULL) {
		LOG(Log::ERROR) << "Cannot open file: " << pidFileName_ << " Error: " << strerror(errno);
		return;
	}
	fprintf(fp, "%d", pid);
	fclose(fp);
}

void Daemon::run() {
	LOG(Log::INFO) << "START";
	if(service_) {
		LOG(Log::INFO) << "RUN";
		service_->run();
	}
	
	LOG(Log::INFO) << "STOP";
	if(fs::exists(pidFileName_))
		fs::remove(pidFileName_);
}

bool Daemon::loadAndVlidateConfigFile(const std::string& configFile) {
	if(!fs::exists(configFile)) {
		std::cerr << "Config file: \"" << configFile << "\" doesn't exists!" << std::endl;
		return false;
	}
	
	if(configFile.at(0) != '/') {
		std::cerr << "Please specify absolute path to config file!" << std::endl;
		return false;
	}
	
	boost::property_tree::ptree configurationTree;
	pt::read_xml(configFile, configurationTree);
	name_ = configurationTree.get<std::string>("configuration.main.name", "");
	if(name_.empty()) {
		std::cerr << "Please specify main.name in configuration file!" << std::endl;
		return false;
	}
	
	pidFileName_ = configurationTree.get<std::string>("configuration.main.pid_file_name", "");
	if(pidFileName_.empty()) {
		std::cerr << "Please specify main.pid_file_name in configuration file!" << std::endl;
		return false;
	}
	
	if(pidFileName_.at(0) != '/') {
		std::cerr << "Please specify absolute path in main.pid_file_name in configuration file!" << std::endl;
		return false;
	}
	
	directoryName_ = configurationTree.get<std::string>("configuration.log.directory_name", "");
	if(directoryName_.empty()) {
		std::cerr << "Please specify log.directory_name in configuration file!" << std::endl;
		return false;
	}
	
	if(directoryName_.at(0) != '/') {
		std::cerr << "Please specify absolute path in log.directory_name in configuration file!" << std::endl;
		return false;
	}
	
	fileName_ = configurationTree.get<std::string>("configuration.log.file_name", "");
	if(fileName_.empty()) {
		std::cerr << "Please specify log.file_name in configuration file!" << std::endl;
		return false;
	}
	
	level_ = configurationTree.get<std::string>("configuration.log.level", "");
	if(level_.empty()) {
		std::cerr << "Please specify log.level in configuration file!" << std::endl;
		return false;
	}
	
	format_ = configurationTree.get<std::string>("configuration.log.format", "");
	if(level_.empty()) {
		std::cerr << "Please specify log.format in configuration file!" << std::endl;
		return false;
	}
	
	rotationSize_ = configurationTree.get<unsigned long>("configuration.log.rotation_size", 0);
	if(rotationSize_ == 0) {
		std::cerr << "Please specify log.rotation_size in configuration file!" << std::endl;
		return false;
	}
	
	return true;
}

void Daemon::main(int argc, char **argv) {
	std::string configFile;
	po::options_description description("Options");
	description.add_options()
		("help", "Show help message.")
		("config-file", po::value<std::string>(&configFile), "Config file.")
		("start", "Start daemon.")
		("stop", "Stop daemon.")
		("restart", "Restart daemon.");
		
	po::variables_map variablesMap;
	po::store(po::parse_command_line(argc, argv, description), variablesMap);
	po::notify(variablesMap);
	
	if(variablesMap.count("config-file") == 0) {
		std::cerr << "Please specify config-file!" << std::endl;
	}
	
	if(!loadAndVlidateConfigFile(configFile)) {
		return;
	}
	
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