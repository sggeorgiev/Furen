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
#include "Message.h"
#include "Session.h"

namespace Transport {
	
class ConnectionCloseEvent: public Processing::Event {
public:
	static const Processing::EventId id;
public:
	ConnectionCloseEvent(const SessionPtr& session, const Utilities::ErrorPtr& error);
	
public:
	virtual const Processing::EventId& getId() const;
	const SessionPtr& getSession() const;
	const Utilities::ErrorPtr& getError() const;
	
private:
	SessionPtr session_;
	Utilities::ErrorPtr error_;
};

};