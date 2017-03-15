#pragma once

#include <string>
#include <stdexcept>

#include "ErrorCode.h"

namespace Log {

class Exception: public std::logic_error {
public:
	Exception(unsigned int errorCode, const std::string& errorMessage);
	
	unsigned int getErrorCode() const;

private:
	unsigned int errorCode_;
};

};