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

//socket ����
typedef enum{
	SOCKTYPE_NONE,
	
	SOCKTYPE_CLIENT, //�ͻ���
	
	SOCKTYPE_GATESERVER,  //���ط�����
	SOCKTYPE_DISPATCHLOGSERVER, //�ַ�����־������
	SOCKTYPE_DATASERVER,  //���ݿ������
	SOCKTYPE_BILLSERVER,  //���ѷ�����
	SOCKTYPE_LOGICSERVER, //�߼�����������Ϸ��Ҫ�߼�����

	SOCKTYPE_ADMIN, //����Ա

	SOCKTYPE_MAX
}SOCKTYPE;


/*
msg head ����
msg head��     [0][1]          [2]           [3]
                 ||             |             |
            ��ʾmsg�ܳ���      cmd          action
*/
#define PACK_HEAD_BYTES 4

/*
msg �ܳ�����ռ�ֽڣ�ռ2�ֽڱ�ʾ���� msg ���ܳ���
*/
#define PACK_LAN_BYTES 2


//�����ݶ�ȡ״̬
#define MSG_LEN_NOT_READED  0 //δ����msg����
#define MSG_LEN_READED  1     //�Ѷ���msg����
#define MSG_READED  2         //msg���� 

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
		legal(true) //����֤��ʱ��Ĭ��ֵΪfalse�ȷǷ�
	{
	}

	~Sock()
	{
		uv_close((uv_handle_t*)handler, 0);
	}

	//socket id
	int32_t id;

	//�յ����ݣ��Է�����Ϣ���ܳ���
	uint16_t len_total;

	//�Ѿ����������ݳ���
	uint16_t len_readed;

	//�洢����������
	char *buf;

	//socket����
	uv_tcp_t* handler;

	//��ǰ���������ݵ�״̬
	int8_t status;
	
	//��timer��Ҫ�����ӵ���֤��ʱ������Ĭ�ϷǷ����Ƿ���������������
	//��xxʱ����session��֤ͨ��תΪ�Ϸ������ܽ��������շ������������ʱδ��֤ͨ�����ߵ���
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
	SOCKTYPE socktype; //server����
	std::string ip; //ip
	int16_t port; //port
	uint16_t connections;  //����������Ҫ�����߼���������logic server������¼�߼��������ͻ������������������ؾ���
	
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

	Sock *sock;     //Msg��Դ��sock->handler
	char *msg;    //��Ϣ�壺ȥ������Msg��head�ĸ��ֽ�
	uint16_t len;   //SockMsg.msgָ���buf����
	int16_t error;  //������
	int8_t cmd;     //���cmd<0, SockMsg.msg��ǰ2���ֽڱ�ʾerror��, error = *(int16_t*)msg
	uint8_t action; //��Ӧcmd�µ���ָ���
};

extern Sock *gLogSock;

#endif // _COMMEN_H_
