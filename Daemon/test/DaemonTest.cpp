#include "Daemon.h"
#include <Log.h>
#include <stdio.h>
#include <boost/property_tree/xml_parser.hpp>

class TestServer: public Server {
public:
        virtual void run() {
		FILE* fp = fopen("/tmp/test-daemon.output", "w");
		for(int i=0; i<1000; i++) {
			fprintf(fp, "%d\n", i);
			fflush(fp);
			sleep(3);
		}
	}
};

int main(int argc, char **argv) {
	ServerPtr server(new TestServer());
	DaemonPtr daemon(new Daemon("daemon_test.xml", server));
	daemon->main(argc, argv);
	return 0;
}