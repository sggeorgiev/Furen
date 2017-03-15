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
#include <boost/shared_ptr.hpp>

namespace Log {

class File {
private:
	static const std::string FILE_EXTENSION;
public:
	File(const std::string& directoryPath, const std::string& fileName, unsigned long rotationSize);
	virtual ~File();
	
public:
	virtual void write(const std::string& data);
	
private:
	typedef boost::shared_ptr<FILE> FilePtr;
	const FilePtr& getFile();
	static void closeFile(FILE *file);
	unsigned long getLastFileIndex();
	void rotate();
	
private:
	std::string directoryPath_;
	std::string fileName_;
	unsigned long rotationSize_;
	FilePtr currentFile_;
	unsigned long currentFileIndex_;
};

typedef boost::shared_ptr<File> FilePtr;

};