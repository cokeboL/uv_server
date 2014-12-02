#include <stdio.h>

#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
#include <stdlib.h>

const int CMD_REGISTER_GS = 10086;

static int gate_port = 0;
static int self_port = 0;
static uv_tcp_t gate_socket;

static void start_server()
{
	start_listen(self_port);

	start_rpc();

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	close_listener();
}

static void send_handler(uv_write_t* req, int status)
{
	if (status == 0 && req)
	{
		//free(req->data);
		free(req);
	}
}

static void on_connect_gate(uv_connect_t* req, int status)
{
	const int packLen = 32;
	char send_buf[packLen] = {0};
	*(short*)send_buf = packLen;
	*(short*)(send_buf+2) = CMD_REGISTER_GS;
	sprintf(send_buf+4, "127.0.0.1:%d", self_port);

	uv_buf_t uv_buf;
	uv_buf.base = send_buf;
	uv_buf.len = packLen;

	uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
	write_req->data = send_buf;

	if(0 == uv_write(write_req, (uv_stream_t*)&gate_socket, &uv_buf, 1, send_handler))
	{
		//free(write_req);
	}

	//start_server();
}

static void connect_gate() 
{
	//**********************
	uv_loop_t *loop = uv_default_loop();
	uv_tcp_init(loop, &gate_socket);

	uv_connect_t connect;

	struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", gate_port);

	int ret = uv_tcp_connect(&connect, &gate_socket, dest, on_connect_gate);

	uv_run(loop, UV_RUN_DEFAULT);
}
	
	
int main(int argc, char **argv)
{
	gate_port = 7000;//atoi(argv[1]);
	self_port = 7001;//atoi(argv[2]);

	connect_gate();

	start_server();

	printf("xxx");

    return 0;
}
