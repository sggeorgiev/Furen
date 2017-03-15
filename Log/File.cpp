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

#include "File.h"
#include "Exception.h"
#include <boost/concept_check.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

namespace Log {

namespace fs = boost::filesystem;

const std::string File::FILE_EXTENSION = ".log";

File::File(const std::string& directoryPath, const std::string& fileName, unsigned long rotationSize): directoryPath_(directoryPath), fileName_(fileName), rotationSize_(rotationSize)  {
	currentFileIndex_ = getLastFileIndex();
}

File::~File() {
};

void File::write(const std::string& data) {
	File::FilePtr file = getFile();
	if(data.length() != fwrite(data.c_str(), sizeof(char), data.length(), file.get())) {
		std::string message = "Cannot write file data!";
		throw Exception(ErrorCode::CANNOT_WRITE_TO_FILE, message);
	}
	
	fflush(file.get());
	
	rotate();
}

void File::rotate() {
	if(!currentFile_)
		return;
	
	long size = ftell(currentFile_.get());
	if(size == -1) {
		std::string message = "Cannot get file position!";
		throw Exception(ErrorCode::CANNOT_GET_FILE_POSITION, message);
	}
	
	if((unsigned long)size > rotationSize_) {
		currentFile_.reset();
		currentFileIndex_ ++;
	}
}

const File::FilePtr& File::getFile() {
	if(currentFile_)
		return currentFile_;
	
	if(!fs::exists(directoryPath_)) {
		fs::create_directories(directoryPath_);
	}
	
	FILE* fp = NULL;
	std::string realFileName = directoryPath_ + "/" + fileName_ + "_" + boost::lexical_cast<std::string>(currentFileIndex_) + FILE_EXTENSION;
	if(fs::exists(realFileName))
		fp = fopen(realFileName.c_str(), "a");
	else
		fp = fopen(realFileName.c_str(), "w");
	
	if(fp == NULL) {
		std::string message = "Cannot open file: " + realFileName;
		throw Exception(ErrorCode::CANNOT_OPEN_FILE, message);
	}
	currentFile_ = FilePtr(fp, closeFile);
	return currentFile_;
}

void File::closeFile(FILE *file) {
	if(file)
		fclose(file);
}

unsigned long File::getLastFileIndex() {
	if(!fs::exists(directoryPath_)) {
		return 0;
	}
	currentFileIndex_ = 0;
	fs::directory_iterator end;
	for(fs::directory_iterator dirWalk(directoryPath_); dirWalk != end; ++dirWalk) {
		if(!fs::is_regular_file(dirWalk->path()))
			continue;
		
		std::string fileName = dirWalk->path().filename().string();
		std::size_t foundName = fileName.find(fileName_ + "_");
		if(foundName == std::string::npos)
			continue;
		
		std::size_t foundExtension = fileName.find(FILE_EXTENSION);
		if(foundExtension == std::string::npos)
			continue;
		
		std::string index = fileName.substr(foundName + fileName_.length() + 1, foundExtension - (fileName_.length() + 1));
		unsigned long newIndex = boost::lexical_cast<unsigned long>(index);
		if(currentFileIndex_ < newIndex)
			currentFileIndex_ = newIndex;
	}
	
	return currentFileIndex_;
}

};