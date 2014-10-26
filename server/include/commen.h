#ifndef _COMMEN_H_
#define _COMMEN_H_

#if defined(_WIN32)
    #pragma comment(lib,"ws2_32.lib")
#else

#endif

#include "uv.h"


class SOCKMSG
{
public:
	SOCKMSG(int idx_, char *msg_):
		idx(idx_),
		msg(msg_)
	{
	}

	int idx;
	char *msg;
};


#endif // _COMMEN_H_
