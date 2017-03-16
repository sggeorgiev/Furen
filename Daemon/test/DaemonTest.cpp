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
			
			LOG(Log::TRACE) << i;
			
			sleep(3);
		}
	}
};

int main(int argc, char **argv) {
	ServerPtr server(new TestServer());
	DaemonPtr daemon(new Daemon(server));
	daemon->main(argc, argv);
	return 0;
}