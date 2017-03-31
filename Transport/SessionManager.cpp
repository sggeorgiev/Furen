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

#include "include/SessionManager.h"

namespace Transport {
	
SessionManager::SessionManager() {
}

SessionManager::~SessionManager() {
}

void SessionManager::addSession(const SessionId& sessionId, const SessionPtr& session) {
	sessionMap_.insert(std::make_pair(sessionId, session));
}

SessionPtr SessionManager::getSession(const SessionId& sessionId) const {
	SessionMap::const_iterator it = sessionMap_.find(sessionId);
	if(it != sessionMap_.end())
		return it->second;
	return SessionPtr();
}

};