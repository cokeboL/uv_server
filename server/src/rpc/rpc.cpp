#include "rpc.h"
#include <queue>

//lock
static uv_mutex_t rpc_queue_mutex;

//sem
static uv_sem_t rpc_queue_sem;

//msg queue
static std::queue<SOCKMSG*> rpc_queue;

void rpc_push_msg(SOCKMSG* msg)
{
	uv_mutex_lock(&rpc_queue_mutex);
	rpc_queue.push(msg);
	uv_mutex_unlock(&rpc_queue_mutex);
	uv_sem_post(&rpc_queue_sem);
}