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

#include "include/Client.h"
#include <Log.h>
#include <ErrorCode.h>

namespace Transport {

Client::Client(IOServeice& ioServeice, const EndpointIterator& endpointIterator): 
	ioServeice_(ioServeice),
	endpointIterator_(endpointIterator),
	socket_(ioServeice) {
	
}

Client::~Client() {
}

void Client::start(const ReadCallback& readCallback) {
	readCallback_ = readCallback;
	message_.reset(new Message);
	boost::asio::async_connect(socket_, endpointIterator_, boost::bind(&Client::handleConnect, shared_from_this(), boost::asio::placeholders::error));
}

void Client::handleConnect(const boost::system::error_code& errorCode) {
	if (!errorCode) {
		message_.reset(new Message);
		boost::asio::async_read(socket_, boost::asio::buffer(message_->getData(), Message::HEADER_SIZE), boost::bind(&Client::handleReadMessageHeader, shared_from_this(), boost::asio::placeholders::error));
	}
	else {
		Utilities::ErrorPtr error(new Utilities::Error(Utilities::ErrorCode::CANNOT_CONNECT_TO_SERVER, errorCode.message()));
		readCallback_(message_, error);
	}
}

void Client::handleReadMessageHeader(const boost::system::error_code& errorCode) {
	if (!errorCode && message_->decodeHeader()) {
		LOG(Log::DEBUG) << "readMessageHeader complete successful, body size: " << message_->getBodyLength();
		boost::asio::async_read(socket_, boost::asio::buffer(message_->getBody(), message_->getBodyLength()), boost::bind(&Client::handleReadMessageBody, shared_from_this(), boost::asio::placeholders::error));
	}
	else {
		LOG(Log::ERROR) << "readMessageHeader fail: " << errorCode;
		Utilities::ErrorPtr error(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message()));
		readCallback_(message_, error);
	}
}

void Client::handleReadMessageBody(const boost::system::error_code& errorCode) {
	if (!errorCode) {
		LOG(Log::DEBUG) << "readMessageBody complete successful";
		Utilities::ErrorPtr error;
		readCallback_(message_, error);
		
		message_.reset(new Message);
		boost::asio::async_read(socket_, boost::asio::buffer(message_->getData(), Message::HEADER_SIZE), boost::bind(&Client::handleReadMessageHeader, shared_from_this(), boost::asio::placeholders::error));
	}
	else {
		LOG(Log::ERROR) << "readMessageBody fail: " << errorCode;
		Utilities::ErrorPtr error(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message()));
		readCallback_(message_, error);
	}
}

void Client::write(const WriteCallback& writeCallback, const MessagePtr& message)
{
	bool writeInProgress = !messageItemQueue_.empty();
	MessageItemPtr newMessageItem(new MessageItem);
	newMessageItem->message = message;
	newMessageItem->callback = writeCallback;
	messageItemQueue_.push_back(newMessageItem);
	if (!writeInProgress) {
		MessageItemPtr messageItem = messageItemQueue_.front();
		boost::asio::async_write(socket_, boost::asio::buffer(messageItem->message->getData(), messageItem->message->getLength()), boost::bind(&Client::handleWrite, shared_from_this(), messageItem->callback, boost::asio::placeholders::error));
	}
}

void Client::handleWrite(const WriteCallback& writeCallback, const boost::system::error_code& errorCode) {
	if (!errorCode) {
		LOG(Log::DEBUG) << "write complete successful";
		Utilities::ErrorPtr error;
		writeCallback(error);
		
		messageItemQueue_.pop_front();
		if (!messageItemQueue_.empty()) {
			MessageItemPtr messageItem = messageItemQueue_.front();
			boost::asio::async_write(socket_, boost::asio::buffer(messageItem->message->getData(), messageItem->message->getLength()), boost::bind(&Client::handleWrite, shared_from_this(), messageItem->callback, boost::asio::placeholders::error));
		}
	}
	else {
		LOG(Log::ERROR) << "write fail: " << errorCode;
		writeCallback(Utilities::ErrorPtr(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message())));
	}
}
	
};