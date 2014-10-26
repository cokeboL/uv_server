#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "uv.h"
#include "commen.h"


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

int start_server_at_port(int port);
void close_server();

#endif // _CONNECTOR_H_