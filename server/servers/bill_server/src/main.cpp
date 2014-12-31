#include <stdio.h>

#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
#include "regist/regist.h"
#include <stdlib.h>

#define ASSERT(expr)   
/*
do {                                                     \
	if(!expr)){                                      \
	fprintf(stderr,                                       \
	"Assertion failed in %s on line %d: %s\n",    \
	__FILE__,                                     \
	__LINE__,                                     \
	#expr);                                       \
	abort();                                              \
	}                                                    \
}while(0)
*/

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

typedef struct{
	uv_tcp_t handle;
	uv_shutdown_t shutdown_req;
} conn_rec;

static uv_tcp_t tcp_server;




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


void ipc_on_connection_tcp_conn(uv_stream_t *server, int status) {
    if (status == -1) {
        // error!
        return;
    }

    printf("-- ipc_on_connection_tcp_conn status: %d!\n", status);
}

void connect_child_process_cb(uv_connect_t* req, int status)
{
	printf("-- client on_connect status: %d!\n", status);
}

void start_listen(int port)
{
	uv_tcp_t tcp_server;


	uv_tcp_init(uv_default_loop(), &tcp_server);

	uv_tcp_bind(&tcp_server, uv_ip4_addr("127.0.0.1", 7001));

	uv_listen((uv_stream_t*)&tcp_server, 12, ipc_on_connection_tcp_conn);

}

void regist_to_gate()
{
	uv_connect_t connreq;
	uv_tcp_t conn;
	struct sockaddr_in addr;
	uv_tcp_init(uv_default_loop(), &conn);
	addr = uv_ip4_addr("127.0.0.1", 7000);
	uv_tcp_connect(&connreq, (uv_tcp_t*)&conn, addr, connect_child_process_cb);
}


int main()
{	

	start_rpc();
	start_connector(7001);
	/*
	uv_tcp_init(uv_default_loop(), &connector_server);
	uv_tcp_bind(&connector_server, uv_ip4_addr("127.0.0.1", 7001));
	uv_listen((uv_stream_t*)&connector_server, 12, on_new_connection);


	uv_connect_t conn_gate_req;
	uv_tcp_init(uv_default_loop(), &connector_connect);
	uv_tcp_connect(&conn_gate_req, (uv_tcp_t*)&connector_connect, uv_ip4_addr("127.0.0.1", 7000), on_connect_gate);

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	*/

	// clear
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
