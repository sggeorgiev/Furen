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

#include <iostream>
#include <Log.h>

int main() {	
	Log::init("/tmp/test_log/", "log_test", "trace", "%Time% %Severity% %ProcessId% %ThreadId% %Text%", 104857600);
	
	for(int i=0; i <20; i++) {
		LOG(Log::TRACE) << "test test test" << " tttt " << i;
	}
	
	LOG(Log::DEBUG) << "test test test" << " debug ";
	LOG(Log::ERROR) << "test test test" << " error ";
	LOG(Log::FATAL) << "test test test" << " fatal ";
	LOG(Log::INFO) << "test test test" << " info ";
	
	sleep(20);
	return 0;
}