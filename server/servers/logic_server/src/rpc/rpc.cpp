#include "rpc.h"
#include <queue>

//lock
static uv_mutex_t rpc_recv_queue_mutex;
static uv_mutex_t rpc_send_queue_mutex;

//sem
static uv_sem_t rpc_recv_queue_sem;

//msg queue
static std::queue<SOCKMSG*> rpc_recv_queue;
static std::queue<SOCKMSG*> rpc_send_queue;
/*
void free_msg(SOCKMSG* msg)
{
	free(msg->msg);
	delete msg;
}
*/

void rpc_push_msg(SOCKMSG* msg)
{
	uv_mutex_lock(&rpc_recv_queue_mutex);
	rpc_recv_queue.push(msg);
	uv_mutex_unlock(&rpc_recv_queue_mutex);
	uv_sem_post(&rpc_recv_queue_sem);
}

//处理消息，比如push到lua里处理
static void rpc_recv_handler(uv_work_t *req, SOCKMSG* msg)
{
	printf("id: %d %d %d msg: %s\n", msg->sock->id,msg->cmd, msg->action, msg->msg);
}

static void rpc_recv_loop(uv_work_t *req)
{
	while(1)
	{
		uv_sem_wait(&rpc_recv_queue_sem);
		
		uv_mutex_lock(&rpc_recv_queue_mutex);
		SOCKMSG* msg = rpc_recv_queue.front();
		
		uv_mutex_unlock(&rpc_recv_queue_mutex);

		rpc_recv_handler(req, msg);

		rpc_recv_queue.pop();
		delete msg;
	}
}

static void send_handler(uv_write_t* req, int status)
{
	if (status == 0 && req)
	{
		free(req->data);
		free(req);
	}
}

void rpc_send_loop(uv_async_t *handle, int status )
{
	while(1)
	{
		uv_mutex_lock(&rpc_send_queue_mutex);
		 if (rpc_send_queue.size() == 0) {
		 	uv_mutex_unlock(&rpc_send_queue_mutex);
			uv_sleep(5);
		 	continue;
		 }
		SOCKMSG* msg = rpc_send_queue.front();

		char *send_buf = (char*)malloc(msg->len+4);
		*(short*)send_buf = msg->len + 2;
		*(short*)(send_buf+2) = msg->cmd;
		memcpy(send_buf+4, msg->msg, msg->len);

		uv_buf_t uv_buf;
		uv_buf.base = send_buf;
		uv_buf.len = msg->len + 4;

		uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
		write_req->data = send_buf;

		if (msg->sock)
		{
			if(0 == uv_write(write_req, (uv_stream_t*)msg->sock->handler, &uv_buf, 1, send_handler))
			{
				free(send_buf);
				free(write_req);
				rpc_send_queue.pop();
				delete msg;
			}
		}
		else
		{
			rpc_send_queue.pop();
			delete msg;
		}
		uv_mutex_unlock(&rpc_send_queue_mutex);	
	}
}

static void start_rpc_recv()
{
	static uv_work_t req;
	uv_queue_work(uv_default_loop(), &req, rpc_recv_loop, 0);
}

static void start_rpc_send()
{
	static uv_async_t async;
	uv_async_init(uv_default_loop(), &async, rpc_send_loop);
}

void start_rpc()
{
	uv_sem_init(&rpc_recv_queue_sem, 0);

	uv_mutex_init(&rpc_recv_queue_mutex);
	uv_mutex_init(&rpc_send_queue_mutex);

	start_rpc_recv();
	start_rpc_send();
}