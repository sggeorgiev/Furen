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