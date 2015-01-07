#ifndef _LOAD_BALANCE_H_
#define _LOAD_BALANCE_H_

#include "commen.h"

class LoadBalance
{
public:
	static void HandleMsg(SockMsg* msg);
	static void SimpleBalance(SockMsg* msg);
};

#endif // _LOAD_BALANCE_H_