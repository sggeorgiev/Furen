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

#include "Session.h"
#include <map>

namespace Transport {

class SessionManager {
public:
	SessionManager();
	virtual ~SessionManager();
	
public:
	void addSession(const SessionId& sessionId, const SessionPtr& session);
	SessionPtr getSession(const SessionId& sessionId) const;
	
private:
	typedef std::map<SessionId, SessionPtr> SessionMap;
	SessionMap sessionMap_;
};

typedef boost::shared_ptr<SessionManager> SessionManagerPtr;

};