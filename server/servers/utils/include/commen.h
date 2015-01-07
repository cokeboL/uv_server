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
		handler(client),
		legal(true)
	{
	}

	int id;
	int status;
	int len_total;
	int len_readed;
	char* buf;
	uv_tcp_t* handler;
	bool legal;
};

class ServerSock: public Sock
{
public:
	ServerSock(int _id, uv_tcp_t* client, SOCKTYPE sock_type, std::string &ip_, const int port_): 
		Sock(_id, client),
		port(port_),
		socktype(sock_type)
	{
		ip = ip_;
	}
	ServerSock(Sock &sock, SOCKTYPE sock_type, std::string &ip_, const int port_): 
		Sock(sock),
		port(port_),
		socktype(sock_type)
	{
		ip = ip_;
	}

	~ServerSock()
	{
	}
	SOCKTYPE socktype;
	std::string ip;
	int port;
};

#define HEAD_LEN 4
class SockMsg
{
public:
	SockMsg(Sock *sock_, char cmd_, char action_, const char *msg_, int len_):
		sock(sock_),
		cmd(cmd_),
		action(action_),
		msg(0),
		len(len_),
		error(0)
	{
		if(len>0)
		{
			msg = new char[len];
			memcpy(msg, msg_, len);
		}
	}

	SockMsg(Sock *sock_, char cmd_, char action_, const char *msg_, int len_, short error_):
		sock(sock_),
		cmd(cmd_),
		action(action_),
		msg(0),
		len(len_),
		error(error_)
	{
		if(len>0)
		{
			msg = new char[len];
			memcpy(msg, msg_, len);
		}
	}

	SockMsg(SockMsg &SockMsg):
		sock(SockMsg.sock),
		cmd(SockMsg.cmd),
		action(SockMsg.action),
		len(SockMsg.len),
		error(0)
	{
		if(len>0)
		{
			msg = new char[len];
			memcpy(msg, SockMsg.msg, len);
		}
	}

	~SockMsg()
	{
		delete msg;
	}

	Sock *sock;
	int len;
	char *msg;
	short error;
	char cmd; //when cmd<0, the first 2 bytes of msg marks the error, error = *(short*)msg
	unsigned char action;
};

extern Sock *gLogSock;

#endif // _COMMEN_H_
