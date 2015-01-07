#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "commen.h"

class Logger
{
public:
	//static Logger *getInstance();
	static void HandleMsg(SockMsg* msg);
	static void record(SockMsg* msg);
};

#endif // _LOGGER_H_