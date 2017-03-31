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
#include "Event.h"
#include "Listener.h"
#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include <map>

namespace Processing {

class EventBus: public boost::noncopyable {
public:
	~EventBus();
	static EventBus& instance();
	
	virtual void registerListener(const EventId& eventId, const ListenerPtr& listener);
	virtual void dispatchEvent(const Event& event);
	
protected:
	typedef std::vector<ListenerPtr> ListenerList;
	typedef std::map<EventId, ListenerList> ListenerMap;
	
	ListenerMap listeners_;
	boost::mutex mutex_;
	
	typedef boost::shared_ptr<EventBus> EventBusPtr;
	static EventBusPtr instance_;
	static boost::mutex instanceMutex_;
};
};