#ifndef _LOAD_BALANCE_H_
#define _LOAD_BALANCE_H_

#include "commen.h"

class LoadBalance
{
public:
	static void HandleMsg(SOCKMSG* msg);
	static void SimpleBalance(SOCKMSG* msg);
};

#endif // _LOAD_BALANCE_H_