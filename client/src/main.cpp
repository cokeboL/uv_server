#include <stdio.h>

#include "commen.h"
#include "uv.h"

#include <thread>

void on_connect(uv_connect_t* req, int status)
{
	printf("-- client on_connect!\n");
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
	
	Sleep(100000000);
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
