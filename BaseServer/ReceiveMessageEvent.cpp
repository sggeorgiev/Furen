#include "include/ReceiveMessageEvent.h"

namespace BaseServer {

const Processing::EventId ReceiveMessageEvent::id = "ReceiveMessageEvent";
	
ReceiveMessageEvent::ReceiveMessageEvent(const MessagePtr& message): message_(message) {
}

const Processing::EventId& ReceiveMessageEvent::getId() const {
	return ReceiveMessageEvent::id;
}

const MessagePtr& ReceiveMessageEvent::getMessage() const {
	return message_;
}

};