#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "commen.h"

class Logger
{
public:
	//static Logger *getInstance();
	static void HandleMsg(SOCKMSG* msg);
	static void record(SOCKMSG* msg);
};

#endif // _LOGGER_H_