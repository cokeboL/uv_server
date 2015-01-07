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

class ServerSock: public Sock
{
public:
	ServerSock(int _id, uv_tcp_t* client, SOCKTYPE sock_type, const char *ip_, const int port_): 
		Sock(_id, client),
		ip(0),
		port(port_),
		socktype(sock_type)
	{
	}
	ServerSock(Sock &sock, SOCKTYPE sock_type, const char *ip_, const int port_): 
		Sock(sock),
		ip(0),
		port(port_),
		socktype(sock_type)
	{
		if(ip_)
		{
			ip = new char[strlen(ip_)+1];
			strncpy(ip, ip_, strlen(ip_));
		}
	}

	~ServerSock()
	{
		if(ip)
		{
			delete(ip);
		}
	}
	SOCKTYPE socktype;
	char *ip;
	int port;
};

#define HEAD_LEN 4
class SOCKMSG
{
public:
	SOCKMSG(Sock *sock_, char cmd_, char action_, const char *msg_, int len_):
		sock(sock_),
		cmd(cmd_),
		action(action_),
		msg(0),
		len(len_)
	{
		if(len>0)
		{
			msg = new char[len];
			memcpy(msg, msg_, len);
		}
	}

	SOCKMSG(SOCKMSG &sockmsg):
		sock(sockmsg.sock),
		cmd(sockmsg.cmd),
		action(sockmsg.action),
		len(sockmsg.len)
	{
		if(len>0)
		{
			msg = new char[len];
			memcpy(msg, sockmsg.msg, len);
		}
	}

	~SOCKMSG()
	{
		delete msg;
	}

	Sock *sock;
	int len;
	char *msg;
	unsigned char cmd;
	unsigned char action;
};

extern Sock *gLogSock;

#endif // _COMMEN_H_
