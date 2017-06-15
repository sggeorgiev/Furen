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

#include "include/ConnectionCloseEvent.h"

namespace Transport {

const Processing::EventId ConnectionCloseEvent::id = "ConnectionCloseEvent";
	
ConnectionCloseEvent::ConnectionCloseEvent(const SessionPtr& session, const Utilities::ErrorPtr& error): session_(session), error_(error)  {
}

const Processing::EventId& ConnectionCloseEvent::getId() const {
	return ConnectionCloseEvent::id;
}

const SessionPtr& ConnectionCloseEvent::getSession() const {
	return session_;
}

const Utilities::ErrorPtr& ConnectionCloseEvent::getError() const {
	return error_;
}

};