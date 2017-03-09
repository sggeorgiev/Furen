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
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace pt = boost::property_tree;
namespace expr = boost::log::expressions;

Daemon::Daemon(const std::string& configFileName, const ServerPtr& server): server_(server) {
	pt::read_xml(configFileName, configurationTree_);
	
	//Main configuration
	name_ = configurationTree_.get<std::string>("configuration.main.name");
	pidFileName_ = configurationTree_.get<std::string>("configuration.main.pid_file_name");
		
	initLog();

	BOOST_LOG_SEV(log_, boost::log::trivial::trace) << "A trace severity message";
	BOOST_LOG_SEV(log_, boost::log::trivial::debug) << "A debug severity message";
	BOOST_LOG_SEV(log_, boost::log::trivial::info) << "An informational severity message";
	BOOST_LOG_SEV(log_, boost::log::trivial::warning) << "A warning severity message";
	BOOST_LOG_SEV(log_, boost::log::trivial::error) << "An error severity message";
	BOOST_LOG_SEV(log_, boost::log::trivial::fatal) << "A fatal severity message";
}

Daemon::~Daemon() {
}

void Daemon::start() {
	BOOST_LOG_SEV(log_, boost::log::trivial::info) << "START";
	
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
	BOOST_LOG_SEV(log_, boost::log::trivial::info) << "STOP";
	
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
	BOOST_LOG_SEV(log_, boost::log::trivial::info) << "RESTART";
	
	stop();
	start();
}

void Daemon::daemonize() {
	boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
	
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
			BOOST_LOG_SEV(log_, boost::log::trivial::error) << "First fork failed!";			
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
			BOOST_LOG_SEV(log_, boost::log::trivial::error) << "Second fork failed!";
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
		BOOST_LOG_SEV(log_, boost::log::trivial::error) << "Unable to open /dev/null!";
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

void Daemon::initLog() {
	//Log configuration
	std::string fileName = configurationTree_.get<std::string>("configuration.log.file_name");
	std::string level = configurationTree_.get<std::string>("configuration.log.level");
	unsigned long rotationSize = configurationTree_.get<unsigned long>("configuration.log.rotation_size");
	unsigned char timeBasedRotationHour = configurationTree_.get<unsigned char>("configuration.log.time_based_rotation.hour");
	unsigned char timeBasedRotationMinute = configurationTree_.get<unsigned char>("configuration.log.time_based_rotation.minute");
	unsigned char timeBasedRotationSecond = configurationTree_.get<unsigned char>("configuration.log.time_based_rotation.second");

	boost::log::add_file_log
	(
		boost::log::keywords::open_mode = std::ios_base::app,
		boost::log::keywords::file_name = fileName + "_%N.log",
		boost::log::keywords::rotation_size = rotationSize,
		boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(timeBasedRotationHour, timeBasedRotationMinute, timeBasedRotationSecond),
		boost::log::keywords::format =
		(
			expr::stream
			<< std::setw(8) << std::setfill('0') 
			<< expr::attr< unsigned int >("LineID")
			<< " "
			<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S.%f")
			<< " <" << boost::log::trivial::severity
			<< "> \t" << expr::smessage
		)
	);

	boost::log::core::get()->set_filter
	(
		boost::log::trivial::severity >= getLogLevel(level)
	);
	
	boost::log::add_common_attributes();
}

boost::log::trivial::severity_level Daemon::getLogLevel(const std::string& level) {
	if(level == "trace")
		return boost::log::trivial::severity_level::trace;
	else if(level == "debug")
		return boost::log::trivial::severity_level::debug;
	else if(level == "info")
		return boost::log::trivial::severity_level::info;
	else if(level == "warning")
		return boost::log::trivial::severity_level::warning;
	else if(level == "error")
		return boost::log::trivial::severity_level::error;
	else if(level == "fatal")
		return boost::log::trivial::severity_level::fatal;
	
	return boost::log::trivial::severity_level::trace;
}