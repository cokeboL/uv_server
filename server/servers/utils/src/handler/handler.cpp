#include "handler/handler.h"
#include "rpc/rpc.h"
#include "log/log.h"
#include "connector/connector.h"
#include <stdlib.h>
#include <fcntl.h>

uv_buf_t alloc_buf(uv_handle_t* handle, size_t suggested_size)
{
	return uv_buf_init((char *)malloc(suggested_size), suggested_size);
}

void read_data(uv_stream_t* handle, ssize_t nread, uv_buf_t buf)
{
	const static int pack_len_bytes = 2;
	const static int pack_head_bytes = 4;

	const static int pack_len_not_readed = 0;
	const static int pack_len_readed = 1;
	const static int pack_msg_readed = 2;

	uv_tcp_t* tcp = (uv_tcp_t*)handle;
	uv_shutdown_t* req = 0;
	Sock *sock = 0;

	uv_rwlock_rdlock(&id_client_map_rwlock);
	sock = client_map[tcp];
	if(!sock)
	{
		sock = server_map[tcp];
	}
	uv_rwlock_rdunlock(&id_client_map_rwlock);
	
	
	if(sock == 0)
	{
		free(buf.base);
		return;
	}
	

	if (nread < 0)
	{
		if (buf.base)
		{
			free(buf.base);
		}
		goto RELEASE;
	}

	if(nread == 0)
	{
		free(buf.base);
		return;
	}

	int i = 0;
	unsigned char cmd = 0;
	unsigned char action = 0;
	unsigned int len = 0;
	while(i < nread)
	{
		switch(sock->status){
		case pack_len_not_readed:
			if(sock->len_readed == 0)
			{
				sock->len_total = 0;
			}
			for(;i<nread;)
			{
				i++;
				sock->len_readed++;
				if(sock->len_readed == pack_head_bytes)
				{
					sock->len_total = *(short*)buf.base;	
					cmd = *(unsigned char*)(buf.base+pack_len_bytes);
					action = *(unsigned char*)(buf.base+pack_len_bytes+1);
					if(sock->len_total > (1<<28) )
					{
						free(buf.base);
						goto RELEASE;
					}
					sock->buf = (char*)malloc(sock->len_total - pack_head_bytes);
					sock->status = pack_len_readed;
					//sock->len_readed = 0;
					break;
				}

			}
			/*if (i == nread)
			{
				free(buf.base);
				return;
			}
			*/
		case pack_len_readed:
			len = sock->len_total - sock->len_readed < (nread-i) ? (sock->len_total - sock->len_readed) : (nread-i);
			if (len > 0) {
				memcpy(sock->buf, buf.base + i, len);
				sock->len_readed += len;
				i += len;
			}
			if (sock->len_readed == sock->len_total)
			{
				sock->status = pack_msg_readed;
			}
			/*
			else
			{
				free(buf.base);
				return;
			}
			*/
		case pack_msg_readed:
			int len = sock->len_total - pack_head_bytes;

			SOCKMSG *sock_msg = new SOCKMSG(sock, cmd, action, sock->buf, len);

			free(sock->buf);
			sock->buf = 0;

			rpc_push_recv_msg(sock_msg);
			
			sock->status = pack_len_not_readed;
			sock->len_readed = 0;
		}
	}
	free(buf.base);
	return;

RELEASE:
	if(sock)
	{
		uv_rwlock_wrlock(&id_client_map_rwlock);
		id_map.erase(sock->id);
		client_map.erase(sock->handler);
		uv_rwlock_wrunlock(&id_client_map_rwlock);
		
		if(sock->buf)
		{
			free(sock->buf);
		}
		delete sock;
	} 
	uv_close((uv_handle_t*)handle, close_cb);	
}


void send_handler(uv_write_t* req, int status)
{
	if (status == 0 && req)
	{	
	}
	else
	{
	}
	free(req->data);
	free(req);
}


void close_cb(uv_handle_t* handle)
{
}


void getServersInfo()
{
	FILE *fp = fopen("./config", "r");
	if(!fp)
	{
		return;
	}

	char buf[512];

	while(fgets(buf, sizeof(buf), fp))
	{
		if(strncmp(buf, "gate", strlen("gate")) == 0)
		{
			std::string s = std::string(buf);
			IP_GATESERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_GATESERVER = atoi(s.c_str());
												
		}
		else if(strncmp(buf, "dispatch_log", strlen("dispatch_log")) == 0)
		{
			std::string s = std::string(buf);
			IP_DISPATCHLOGSERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_DISPATCHLOGSERVER = atoi(s.c_str());
		}
		else if(strncmp(buf, "data", strlen("data")) == 0)
		{
			std::string s = std::string(buf);
			IP_DATASERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_DATASERVER = atoi(s.c_str());
		}
		else if(strncmp(buf, "bill", strlen("bill")) == 0)
		{
			std::string s = std::string(buf);
			IP_BILLSERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_BILLSERVER = atoi(s.c_str());
		}
		else if(strncmp(buf, "logic", strlen("logic")) == 0)
		{
			std::string s = std::string(buf);
			IP_LOGICSERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_LOGICSERVER = atoi(s.c_str());
		}
	}
}

