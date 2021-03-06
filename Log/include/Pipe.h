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

#pragma once

#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

namespace Log {

class Pipe {
private:
	static const unsigned int BUFFER_SIZE = 256;
public: 
	Pipe();
	~Pipe();

public:
	void write(const std::string& data);
	std::string read();
	
private:
	int fd_[2];
};

}