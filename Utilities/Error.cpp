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

#include "include/Error.h"

namespace Utilities {
	
Error::Error(unsigned int code, const std::string& message): code_(code), message_(message) {
}

Error::~Error() {
}

unsigned int Error::getCode() const {
	return code_;
}

const std::string& Error::getMessage() const {
	return message_;
}

std::ostream& operator<< (std::ostream& stream, const Error& error) {
	stream << "Error: " << error.getCode() << ", " << error.getMessage();
	return stream; 
}

};