#include "Daemon.h"
#include <stdio.h>

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
	DaemonPtr daemon(new Daemon("/tmp/test-daemon.pid", server));
	daemon->main(argc, argv);
	return 0;
}