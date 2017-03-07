#pragma once

#include <boost/shared_ptr.hpp>

class Server {
public:
	virtual ~Server() {};
	virtual void run() {};
};

typedef boost::shared_ptr<Server> ServerPtr;