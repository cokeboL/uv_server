#include <stdio.h>

#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
#include "regist/regist.h"
#include <stdlib.h>




uv_timer_t timer;
void tickHandler(uv_timer_t *req, int status) 
{
	
	static unsigned int count = 0;
	count++;
	printf("-- tickHandler count: %d\n", count);
	if(uv_is_active((uv_handle_t*) req))
	{
		printf("------ alive\n");
	}
	else
	{
		printf("------ not alive\n");
	}
	//uv_close((uv_handle_t*) &timer, close_cb);

}

void tickHandler2(uv_timer_t *req, int status) 
{
	
	static unsigned int count = 0;
	count++;
	printf("-- tickHandler2 count: %d\n", count);
	if(uv_is_active((uv_handle_t*) &timer))
	{
		printf("------ alive\n");
	}
	else
	{
		printf("------ not alive\n");
	}


}



int main()
{	
	start_rpc();
	start_connector(7001);
	
	close_rpc();
	close_connector();

    return 0;
}




#if 0
int main()
{
	uv_tcp_t tcp_server;


	uv_tcp_init(uv_default_loop(), &tcp_server);

	uv_tcp_bind(&tcp_server, uv_ip4_addr("127.0.0.1", 7001));

	uv_listen((uv_stream_t*)&tcp_server, 12, ipc_on_connection_tcp_conn);


	

	uv_connect_t connreq;
	uv_tcp_t conn;
	struct sockaddr_in addr;
	uv_tcp_init(uv_default_loop(), &conn);
	addr = uv_ip4_addr("127.0.0.1", 7000);
	uv_tcp_connect(&connreq, (uv_tcp_t*)&conn, addr, connect_child_process_cb);


	uv_run(uv_default_loop(), UV_RUN_DEFAULT);


	//MAKE_VALGRIND_HAPPY();

    return 0;
}
#endif
