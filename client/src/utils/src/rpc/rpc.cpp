#include "rpc/rpc.h"
#include "handler/handler.h"
#include <queue>
#include "commen.h"
#include "MsgHandler/MsgHandler.h"
#include "connector/connector.h"

//lock
static uv_mutex_t rpc_recv_queue_mutex;
uv_mutex_t rpc_send_queue_mutex;

//sem
static uv_sem_t rpc_recv_queue_sem;
//static uv_async_t rpc_send_async;

//msg queue
static std::queue<SockMsg*> rpc_recv_queue;
//static std::queue<SockMsg*> rpc_send_queue;

/*
void rpc_push_msg(SockMsg* msg)
{
	uv_mutex_lock(&rpc_send_queue_mutex);
	rpc_send_queue.push(msg);
	uv_mutex_unlock(&rpc_send_queue_mutex);
	uv_async_send(&rpc_send_async);
}
*/

void rpc_send_msg_without_dele(SockMsg* msg)
{
	/*
	uv_mutex_lock(&rpc_send_queue_mutex);
	SockMsg* msg = rpc_send_queue.front();
	rpc_send_queue.pop();
	uv_mutex_unlock(&rpc_send_queue_mutex);	
	*/
	char *send_buf;
	if(msg->error)
	{
		send_buf = (char*)malloc(msg->len+HEAD_LEN+2);
		*(short*)send_buf = msg->len+HEAD_LEN+2;
		send_buf[2] = -msg->cmd;
		send_buf[3] = msg->action;
		*(short*)(send_buf+4) = msg->error;
		memcpy(send_buf+6, msg->msg, msg->len+2);
	}
	else
	{
		send_buf = (char*)malloc(msg->len+HEAD_LEN);
		*(short*)send_buf = msg->len+HEAD_LEN;
		send_buf[2] = msg->cmd;
		send_buf[3] = msg->action;
		memcpy(send_buf+4, msg->msg, msg->len);
	}
	uv_buf_t uv_buf;
	uv_buf.base = send_buf;
	uv_buf.len = msg->len+HEAD_LEN;

	uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
	write_req->data = send_buf;

	if (msg->sock)
	{
		if(0 == uv_write(write_req, (uv_stream_t*)msg->sock->handler, &uv_buf, 1, send_handler))
		{
			//free(send_buf);
			//free(write_req);
		}
	}
}

void rpc_send_msg(SockMsg* msg)
{
	rpc_send_msg_without_dele(msg);
	delete msg;
}

void rpc_push_recv_msg(SockMsg* msg)
{
	uv_mutex_lock(&rpc_recv_queue_mutex);
	rpc_recv_queue.push(msg);
	uv_mutex_unlock(&rpc_recv_queue_mutex);
	uv_sem_post(&rpc_recv_queue_sem);
}


static void rpc_recv_loop(uv_work_t *req)
{
	while(1)
	{
		uv_sem_wait(&rpc_recv_queue_sem);
		
		uv_mutex_lock(&rpc_recv_queue_mutex);
		SockMsg* msg = rpc_recv_queue.front();
		
		uv_mutex_unlock(&rpc_recv_queue_mutex);

		MsgHandler::HandleMsg(msg);

		rpc_recv_queue.pop();
		delete msg;
	}
}

/*
void rpc_send_loop(uv_work_t *req)
{
	while(1)
	{
		uv_mutex_lock(&rpc_send_queue_mutex);
		 if (rpc_send_queue.size() == 0) {
		 	uv_mutex_unlock(&rpc_send_queue_mutex);
			uv_sleep(5);
		 	continue;
		 }
		SockMsg* msg = rpc_send_queue.front();
		rpc_send_queue.pop();
		uv_mutex_unlock(&rpc_send_queue_mutex);	

		char *send_buf = (char*)malloc(msg->len+HEAD_LEN);
		*(short*)send_buf = msg->len+HEAD_LEN;
		send_buf[2] = msg->cmd;
		send_buf[3] = msg->action;
		memcpy(send_buf+4, msg->msg, msg->len);

		uv_buf_t uv_buf;
		uv_buf.base = send_buf;
		uv_buf.len = msg->len+HEAD_LEN;

		uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
		write_req->data = send_buf;

		if (msg->sock)
		{
			if(0 == uv_write(write_req, (uv_stream_t*)msg->sock->handler, &uv_buf, 1, send_handler))
			{
				//free(send_buf);
				//free(write_req);
			}
		}
		
		delete msg;
	}
}
*/
int chatMsgSendCount = 0;
static void send_chat_msg(uv_work_t *req)
{
	extern ServerSock *gGateSock;
	
	char buf[100] = {0};
	
	while(1)
	{
		uv_sleep(5);
		if(chatMsgSendCount >= 10000)
		{
			uv_sleep(1000);
			continue;
		}
		sprintf(buf, "chat msg %d", ++chatMsgSendCount);
		SockMsg *chatMsg = new SockMsg(gGateSock, CMD_NOTIFY, NOTIFY_CHAT, buf, sizeof(buf));
		rpc_send_msg(chatMsg);
		
	}
}

static void start_rpc_recv()
{
	static uv_work_t req;
	uv_queue_work(uv_default_loop(), &req, rpc_recv_loop, 0);
}


static void start_rpc_send()
{
	static uv_work_t req;
	uv_queue_work(uv_default_loop(), &req, send_chat_msg, 0);
}


void start_rpc()
{
	uv_sem_init(&rpc_recv_queue_sem, 0);

	uv_mutex_init(&rpc_recv_queue_mutex);
	uv_mutex_init(&rpc_send_queue_mutex);

	start_rpc_recv();
	start_rpc_send();
}

void close_rpc()
{
	uv_sem_destroy(&rpc_recv_queue_sem);

	uv_mutex_destroy(&rpc_recv_queue_mutex);
	//uv_mutex_destroy(&rpc_send_queue_mutex);
}