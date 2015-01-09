#ifndef _COMMEN_H_
#define _COMMEN_H_

#if defined(_WIN32)
    #pragma comment(lib,"ws2_32.lib")
#else

#endif

#ifdef _WIN32
#define uv_sleep(timeout) Sleep(timeout)
#define New new

#define Malloc malloc
#define Free free
#else
#define uv_sleep(timeout) usleep(timeout*1000)
#define New new

#define Malloc malloc
#define Free free
#endif

#include "uv.h"
#include "message.h"

#include <iostream>
using namespace std;

//socket 类型
typedef enum{
	SOCKTYPE_NONE,
	
	SOCKTYPE_CLIENT, //客户端
	
	SOCKTYPE_GATESERVER,  //网关服务器
	SOCKTYPE_DISPATCHLOGSERVER, //分发、日志服务器
	SOCKTYPE_DATASERVER,  //数据库服务器
	SOCKTYPE_BILLSERVER,  //付费服务器
	SOCKTYPE_LOGICSERVER, //逻辑服务器：游戏主要逻辑功能

	SOCKTYPE_ADMIN, //管理员

	SOCKTYPE_MAX
}SOCKTYPE;


/*
msg head 长度
msg head：     [0][1]          [2]           [3]
                 ||             |             |
            表示msg总长度      cmd          action
*/
#define PACK_HEAD_BYTES 4

/*
msg 总长度所占字节：占2字节表示单个 msg 的总长度
*/
#define PACK_LAN_BYTES 2


//收数据读取状态
#define MSG_LEN_NOT_READED  0 //未读到msg长度
#define MSG_LEN_READED  1     //已读到msg长度
#define MSG_READED  2         //msg收完 

class Sock
{
public:
	Sock(int32_t _id, uv_tcp_t* client) :
		id(_id), 
		status(MSG_LEN_NOT_READED),
		len_total(0), 
		len_readed(0), 
		buf(0), 
		handler(client),
		legal(true) //加认证超时后默认值为false既非法
	{
	}

	~Sock()
	{
		uv_close((uv_handle_t*)handler, 0);
	}

	//socket id
	int32_t id;

	//收到数据，对方此消息的总长度
	uint16_t len_total;

	//已经读到的数据长度
	uint16_t len_readed;

	//存储读到的数据
	char *buf;

	//socket连接
	uv_tcp_t* handler;

	//当前读到的数据的状态
	int8_t status;
	
	//加timer后要做连接的认证超时，连接默认非法，非法连接来包丢弃；
	//在xx时间内session认证通过转为合法，才能进行正常收发包处理，如果超时未认证通过，踢掉；
	bool legal;
};


class ServerSock: public Sock
{
public:
	ServerSock(int _id, uv_tcp_t* client, SOCKTYPE sock_type, std::string &ip_, const int port_) :
		Sock(_id, client),
		ip(ip_),
		port(port_),
		socktype(sock_type),
		connections(0)
	{
	}
	ServerSock(Sock &sock, SOCKTYPE sock_type, std::string &ip_, const int port_): 
		Sock(sock),
		ip(ip_),
		port(port_),
		socktype(sock_type),
		connections(0)
	{
	}

	~ServerSock()
	{
	}
	SOCKTYPE socktype; //server类型
	std::string ip; //ip
	int16_t port; //port
	uint16_t connections;  //连接数，主要用于逻辑服务器（logic server），记录逻辑服务器客户端连接数用来做负载均衡
	
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
			msg = (char*)Malloc(len);
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
			msg = (char*)Malloc(len);
			memcpy(msg, msg_, len);
		}
	}

	SockMsg(SockMsg &sockmsg):
		sock(sockmsg.sock),
		cmd(sockmsg.cmd),
		action(sockmsg.action),
		len(sockmsg.len),
		error(0)
	{
		if(len>0)
		{
			msg = (char*)Malloc(len);
			memcpy(msg, sockmsg.msg, len);
		}
	}

	~SockMsg()
	{
		if (msg)
		{
			Free(msg);
		}
	}

	Sock *sock;     //Msg来源：sock->handler
	char *msg;    //消息体：去掉整个Msg的head四个字节
	uint16_t len;   //SockMsg.msg指向的buf长度
	int16_t error;  //错误码
	int8_t cmd;     //如果cmd<0, SockMsg.msg的前2个字节表示error号, error = *(int16_t*)msg
	uint8_t action; //对应cmd下的子指令号
};

extern Sock *gLogSock;

#endif // _COMMEN_H_
