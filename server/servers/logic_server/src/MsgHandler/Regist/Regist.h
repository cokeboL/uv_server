#ifndef _REGIST_H_
#define _REGIST_H_

#include "commen.h"

class Regist
{
public:
	//static Regist *getInstance();
	static void HandleMsg(SOCKMSG* msg);
};

#endif // _REGIST_H_