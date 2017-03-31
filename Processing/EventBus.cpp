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

#include "include/EventBus.h"
#include "Log.h"

namespace Processing {

EventBus::EventBusPtr EventBus::instance_;
boost::mutex EventBus::instanceMutex_;

EventBus::~EventBus() {
}

EventBus& EventBus::instance() {
	if(instance_.get() == 0) {
		boost::lock_guard<boost::mutex> guard(instanceMutex_);
		if(instance_.get() == 0) {
			instance_.reset( new EventBus );
		}
	}
        
        return *instance_;
}

void EventBus::registerListener(const EventId& eventId, const ListenerPtr& listener) {
	LOG(Log::DEBUG) << "register listener for event id: " << eventId;
	
	boost::lock_guard<boost::mutex> guard(mutex_);
	ListenerMap::iterator it = listeners_.find(eventId);
	if(it == listeners_.end()) {
		ListenerList listenerList;
		listenerList.push_back(listener);
		listeners_.insert(std::make_pair(eventId, listenerList));
	}
	else {
		it->second.push_back(listener);
	}
}

void EventBus::dispatchEvent(const Event& event) {
	LOG(Log::DEBUG) << "dispatch event with id: " << event.getId();
	
	boost::lock_guard<boost::mutex> guard(mutex_);
	ListenerMap::iterator it = listeners_.find(event.getId());
	if(it == listeners_.end()) {
		LOG(Log::ERROR) << "no event listener for event id: " << event.getId();
		return;
	}
	
	for(ListenerPtr listenerWalk: it->second) {
		listenerWalk->notify(event);
	}
}

};