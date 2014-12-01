#include <stdio.h>

#include "commen.h"
#include "uv.h"

#include <thread>
#include <math.h>

void on_connect(uv_connect_t* req, int status)
{
	printf("-- client on_connect status: %d!\n", status);
}


static void send_handler(uv_write_t* req, int status)
{
	if (status == 0 && req)
	{
		free(req->data);
		free(req);
	}
	printf("xxx\n");
}


void foo() 
{
	uv_tcp_t socket;
	uv_loop_t *loop = uv_default_loop();
	uv_tcp_init(loop, &socket);

	uv_connect_t connect;

	struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", 7000);

	
	int ret = uv_tcp_connect(&connect, &socket, dest, on_connect);
	printf("ret: %d\n", ret);
	uv_run(loop, UV_RUN_DEFAULT);
	
	unsigned int packNum = 0;
	unsigned int packLen = 30;
	while(1)
	{
		packNum++;
		char *send_buf = (char*)malloc(packLen);
		*(short*)send_buf = packLen;
		*(short*)(send_buf+2) = packNum;
		printf("%d %d\n", *(short*)send_buf, *(short*)(send_buf+2));
		for(int i=0; i<packLen-4; i++)
		{
			send_buf[i+4] = 'a';
		}

		uv_buf_t uv_buf;
		uv_buf.base = send_buf;
		uv_buf.len = packLen;

		uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
		write_req->data = send_buf;

		if(0 == uv_write(write_req, (uv_stream_t*)&socket, &uv_buf, 1, send_handler))
		{
			free(send_buf);
			free(write_req);
		}

		Sleep(5);
	}
}

int main()
{
	/*
    uv_tcp_t socket;
	uv_loop_t *loop = uv_default_loop();
	uv_tcp_init(loop, &socket);

	uv_connect_t connect;

	struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", 7000);

	
	int ret = uv_tcp_connect(&connect, &socket, dest, on_connect);
	printf("ret: %d\n", ret);
	uv_run(loop, UV_RUN_DEFAULT);
	*/
	std::thread client(foo); 
	/*
	for(int i=0; i<1000; i++)
	{
		std::thread first(foo); 
		break;
	}*/
	client.join();
}
