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
#include "message.h"

#include <iostream>
using namespace std;

typedef enum{
	PORTTYPE_BILLSERVER=9000,
	PORTTYPE_DATASERVER=9100,
	PORTTYPE_GATESERVER=9200,
	PORTTYPE_DISPATCHLOGSERVER=9300,
	PORTTYPE_LOGICSERVER=9400,
}PORTTYPE;

typedef enum{
	SOCKTYPE_NONE,
	
	SOCKTYPE_CLIENT,
		
	SOCKTYPE_GATESERVER,
	SOCKTYPE_DISPATCHLOGSERVER,
	SOCKTYPE_DATASERVER,
	SOCKTYPE_BILLSERVER,
	SOCKTYPE_LOGICSERVER,
	
	SOCKTYPE_MAX
}SOCKTYPE;

class Sock
{
public:
	Sock(int _id, uv_tcp_t* client): 
		id(_id), 
		status(0), 
		len_total(0), 
		len_readed(0), 
		buf(0), 
		handler(client),
		socktype(SOCKTYPE_CLIENT)
	{
	}
	
	/*
	inline void setType(SOCKTYPE t)
	{
		socktype = t;
	}
	*/

	int id;
	int status;
	int len_total;
	int len_readed;
	char* buf;
	uv_tcp_t* handler;
	SOCKTYPE socktype;
};

class SOCKMSG
{
public:
	SOCKMSG(Sock *sock_, char cmd_, char action_, char *msg_, int len_):
		sock(sock_),
		cmd(cmd_),
		action(action_),
		msg(msg_),
		len(len_)
	{
	}

	~SOCKMSG()
	{
		free(msg);
	}

	Sock *sock;
	int len;
	char *msg;
	unsigned char cmd;
	unsigned char action;
};

extern Sock *gLogSock;

#endif // _COMMEN_H_
