#include "regist/regist.h"
#include "handler/handler.h"


static uv_tcp_t regist_socket;

void on_connect(uv_connect_t* req, int status)
{
	printf("-- client on_connect status: %d!\n", status);
	/*
	if(status == 0)
	{
		uv_read_start((uv_stream_t*)&regist_socket, alloc_buf, read_data);
	
		int packLen = 8;
		char *send_buf = (char*)malloc(packLen);
		*(short*)send_buf = packLen;
		*(char*)(send_buf+2) = (char)CMD_REGIST;
		*(char*)(send_buf+3) = (char)CMD_NOEN_ACTION;
		*(int*)(send_buf+3) = (int)SOCKTYPE_BILLSERVER;

		uv_buf_t uv_buf;
		uv_buf.base = send_buf;
		uv_buf.len = packLen;

		uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
		write_req->data = send_buf;

		if(0 == uv_write(write_req, (uv_stream_t*)&regist_socket, &uv_buf, 1, send_handler))
		{
			free(send_buf);
			free(write_req);
		}
	}
	*/
}


/*
void regist_to_gate()
{
	
	uv_tcp_init(uv_default_loop(), &regist_socket);
	uv_connect_t connreq;
	uv_tcp_connect(&connreq, (uv_tcp_t*)&regist_socket, uv_ip4_addr("127.0.0.1", 7000), on_connect);
}


void close_register()
{
	uv_close((uv_handle_t*)&regist_socket, 0);

}
*/