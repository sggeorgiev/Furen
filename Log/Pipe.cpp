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