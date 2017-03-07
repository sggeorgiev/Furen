#include "include/Daemon.h"
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

Daemon::Daemon(std::string fileName, ServerPtr server): filename_(fileName), server_(server) {
}

Daemon::~Daemon() {
}

void Daemon::start() {
	//Check for a pidfile to see if the daemon already runs
	
	// Start the daemon
	daemonize();
	run();
}

void Daemon::stop() {
	// Get the pid from the pidfile
	// Try killing the daemon process
}

void Daemon::restart() {
	stop();
	start();
}

void Daemon::daemonize() {
	if (pid_t pid = fork())
	{
		if (pid > 0)
		{
		// We're in the parent process and need to exit.
			exit(0);
		}
		else
		{
			syslog(LOG_ERR | LOG_USER, "First fork failed: %m");
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
	if (pid_t pid = fork())
	{
		if (pid > 0)
		{
			exit(0);
		}
		else
		{
			syslog(LOG_ERR | LOG_USER, "Second fork failed: %m");
			exit(1);
		}
	}

	// Close the standard streams. This decouples the daemon from the terminal
	// that started it.
	close(0);
	close(1);
	close(2);

	// We don't want the daemon to have any standard input.
	if (open("/dev/null", O_RDONLY) < 0)
	{
		syslog(LOG_ERR | LOG_USER, "Unable to open /dev/null: %m");
		exit(1);
	}

	// Send standard output to a log file.
	const char* output = "/tmp/asio.daemon.out";
	const int flags = O_WRONLY | O_CREAT | O_APPEND;
	const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (open(output, flags, mode) < 0)
	{
		syslog(LOG_ERR | LOG_USER, "Unable to open output file %s: %m", output);
		exit(1);
	}

	// Also send standard error to the same log file.
	if (dup(1) < 0)
	{
		syslog(LOG_ERR | LOG_USER, "Unable to dup output descriptor: %m");
		exit(1);
	}
}

void Daemon::run() {
	if(server_)
		server_->run();
}