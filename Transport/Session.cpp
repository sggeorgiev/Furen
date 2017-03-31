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

#include "include/Session.h"
#include "Log.h"

namespace Transport {

Session::Session(IOServeice& ioServeice): socket_(ioServeice) {
}

Session::~Session() {
}

void Session::start(const ReadCallback& readCallback) {
	readCallback_ = readCallback;
	message_.reset(new Message);
	boost::asio::async_read(socket_, boost::asio::buffer(message_->getData(), Message::HEADER_SIZE), boost::bind(&Session::handleReadMessageHeader, shared_from_this(), boost::asio::placeholders::error));
}

Socket& Session::getSocket() {
	return socket_;
}

void Session::handleReadMessageHeader(const boost::system::error_code& errorCode) {
	if (!errorCode && message_->decodeHeader()) {
		LOG(Log::DEBUG) << "readMessageHeader complete successful, body size: " << message_->getBodyLength();
		boost::asio::async_read(socket_, boost::asio::buffer(message_->getBody(), message_->getBodyLength()), boost::bind(&Session::handleReadMessageBody, shared_from_this(), boost::asio::placeholders::error));
	}
	else {
		LOG(Log::ERROR) << "readMessageHeader fail: " << errorCode;
		Utilities::ErrorPtr error(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message()));
		readCallback_(message_, error);
	}
}

void Session::handleReadMessageBody(const boost::system::error_code& errorCode) {
	if (!errorCode) {
		LOG(Log::DEBUG) << "readMessageBody complete successful";
		Utilities::ErrorPtr error;
		readCallback_(message_, error);
		
		message_.reset(new Message);
		boost::asio::async_read(socket_, boost::asio::buffer(message_->getData(), Message::HEADER_SIZE), boost::bind(&Session::handleReadMessageHeader, shared_from_this(), boost::asio::placeholders::error));
	}
	else {
		LOG(Log::ERROR) << "readMessageBody fail: " << errorCode;
		Utilities::ErrorPtr error(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message()));
		readCallback_(message_, error);
	}
}

void Session::write(const WriteCallback& callback, const MessagePtr& message)
{
	bool writeInProgress = !messageItemQueue_.empty();
	MessageItemPtr newMessageItem(new MessageItem);
	newMessageItem->message = message;
	newMessageItem->callback = callback;
	messageItemQueue_.push_back(newMessageItem);
	if (!writeInProgress) {
		MessageItemPtr messageItem = messageItemQueue_.front();
		boost::asio::async_write(socket_, boost::asio::buffer(messageItem->message->getData(), messageItem->message->getLength()), boost::bind(&Session::handleWrite, shared_from_this(), messageItem->callback, boost::asio::placeholders::error));
	}
}

void Session::handleWrite(const WriteCallback& callback, const boost::system::error_code& errorCode) {
	if (!errorCode) {
		LOG(Log::DEBUG) << "write complete successful";
		Utilities::ErrorPtr error;
		callback(error);
		
		messageItemQueue_.pop_front();
		if (!messageItemQueue_.empty()) {
			MessageItemPtr messageItem = messageItemQueue_.front();
			boost::asio::async_write(socket_, boost::asio::buffer(messageItem->message->getData(), messageItem->message->getLength()), boost::bind(&Session::handleWrite, shared_from_this(), messageItem->callback, boost::asio::placeholders::error));
		}
	}
	else {
		LOG(Log::ERROR) << "write fail: " << errorCode;
		callback(Utilities::ErrorPtr(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message())));
	}
}

}