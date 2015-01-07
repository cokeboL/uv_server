#ifndef _MSG_HANDLER_H_
#define _MSG_HANDLER_H_

#include "commen.h"

class MsgHandler
{
public:
	static void HandleMsg(SockMsg* msg);
};

#endif // _MSG_HANDLER_H_
