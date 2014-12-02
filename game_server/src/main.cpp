#include <stdio.h>

#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
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

int main()
{
	
	start_listen(7000);

	start_rpc();

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	close_listener();

    return 0;

}
