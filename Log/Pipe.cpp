#include "Pipe.h"
#include "Exception.h"
#include <string.h>

namespace Log {

Pipe::Pipe() {
	pipe(fd_);
}

Pipe::~Pipe() {
	close(fd_[0]);
	close(fd_[1]);
}

void Pipe::write(const std::string& data) {
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	memcpy(buffer, data.c_str(), std::min((unsigned int)data.length(), BUFFER_SIZE - 1));
	::write(fd_[1], buffer, BUFFER_SIZE);
}

std::string Pipe::read() {
	char buffer[BUFFER_SIZE];
	int bytesLeft = BUFFER_SIZE;
	while(bytesLeft > 0) {
		int numberOfBytes = ::read(fd_[0], buffer, bytesLeft);
		if(numberOfBytes == -1)
			throw Exception(ErrorCode::CANNOT_READ_FROM_FILE, "Cannot read from pipe.");
		
		if(numberOfBytes > 0)
			bytesLeft -= numberOfBytes;
	}
	
	return std::string(buffer);
}

}