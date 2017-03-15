#include "Exception.h"

namespace Log {

Exception::Exception(unsigned int errorCode, const std::string& errorMessage): std::logic_error(errorMessage) {
	errorCode_ = errorCode;
}

unsigned int Exception::getErrorCode() const {
	return errorCode_;
}

}