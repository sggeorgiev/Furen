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

namespace BaseServer {

Session::Session(IOServeice& ioServeice): socket_(ioServeice), message_(new Message) {
}

Session::~Session() {
}

void Session::start() {
	
}

Socket& Session::getSocket() {
	return socket_;
}

void Session::read(const ReadCallback& callback) {
	boost::asio::async_read(socket_, boost::asio::buffer(message_->getData(), Message::HEADER_SIZE), boost::bind(&Session::handleReadMessageHeader, shared_from_this(), callback, boost::asio::placeholders::error));
}

void Session::handleReadMessageHeader(const ReadCallback& callback, const boost::system::error_code& errorCode) {
	if (!errorCode && message_->decodeHeader()) {
		LOG(Log::DEBUG) << "readMessageHeader complete successful, body size: " << message_->getBodyLength();
		boost::asio::async_read(socket_, boost::asio::buffer(message_->getBody(), message_->getBodyLength()), boost::bind(&Session::handleReadMessageBody, shared_from_this(), callback, boost::asio::placeholders::error));
	}
	else {
		LOG(Log::ERROR) << "readMessageHeader fail: " << errorCode;
		Utilities::ErrorPtr error(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message()));
		callback(message_, error);
	}
}

void Session::handleReadMessageBody(const ReadCallback& callback, const boost::system::error_code& errorCode) {
	if (!errorCode) {
		LOG(Log::DEBUG) << "readMessageBody complete successful";
		Utilities::ErrorPtr error;
		callback(message_, error);
		boost::asio::async_read(socket_, boost::asio::buffer(message_->getData(), Message::HEADER_SIZE), boost::bind(&Session::handleReadMessageHeader, shared_from_this(), callback, boost::asio::placeholders::error));
	}
	else {
		LOG(Log::ERROR) << "readMessageBody fail: " << errorCode;
		Utilities::ErrorPtr error(new Utilities::Error(Utilities::ErrorCode::CANNOT_READ_FROM_SOCKET, errorCode.message()));
		callback(message_, error);
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