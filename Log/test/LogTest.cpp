#include <iostream>
#include <Log.h>

int main() {
	Log::init("log_test.xml");
	
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