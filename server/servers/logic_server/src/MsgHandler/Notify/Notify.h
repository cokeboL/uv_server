#ifndef _NOTIFY_H_
#define _NOTIFY_H_

#include "commen.h"

class Notify
{
public:
	//static Regist *getInstance();
	static void HandleMsg(SockMsg* msg);

	static void Chat(SockMsg* msg);
};

#endif // _NOTIFY_H_