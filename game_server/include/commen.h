#ifndef _COMMEN_H_
#define _COMMEN_H_

#if defined(_WIN32)
    #pragma comment(lib,"ws2_32.lib")
#else

#endif

#ifdef _WIN32
#define uv_sleep(timeout) Sleep(timeout)
#else
#define uv_sleep(timeout) usleep(timeout*1000)
#endif

#include "uv.h"


class Sock
{
public:
	Sock(int _id, uv_tcp_t* client): 
		id(_id), 
		status(0), 
		len_total(0), 
		len_readed(0), 
		buf(0), 
		handler(client)
	{
	}

	int id;
	int status;
	int len_total;
	int len_readed;
	char* buf;
	uv_tcp_t* handler;
};

class SOCKMSG
{
public:
	SOCKMSG(Sock *sock_, int cmd_, char *msg_, int len_):
		sock(sock_),
		cmd(cmd_),
		msg(msg_),
		len(len_)
	{
	}

	~SOCKMSG()
	{
		free(msg);
	}

	Sock *sock;
	int cmd;
	char *msg;
	int len;
};


#endif // _COMMEN_H_
