#include "Regist.h"
#include "connector/connector.h"
#include "log/log.h"

/*
static Regist _instance;

Regist *Regist::getInstance()
{
	return &_instance;
}
*/

void Regist::handle_msg(SOCKMSG* msg)
{
	uv_rwlock_wrlock(&id_client_map_rwlock);

	id_map[msg->sock->id] = 0;
	client_map[(uv_tcp_t*)msg->sock->handler] = 0;

	SOCKTYPE socktype = *(SOCKTYPE*)msg->msg;
	switch(socktype)
	{
	case SOCKTYPE_LOGICSERVER:
		server_map[(uv_tcp_t*)msg->sock->handler] = new ServerSock(*msg->sock, *(SOCKTYPE*)msg->msg, "hahaha", 100);
		break;
	default:
		server_map[(uv_tcp_t*)msg->sock->handler] = new ServerSock(*msg->sock, socktype, 0, 0);
		break;
	}
	
	
	/*
	std::cout << "server type: " << server_map[(uv_tcp_t*)msg->sock->handler]->socktype
		<< " " << server_map[(uv_tcp_t*)msg->sock->handler]->port
		<< std::endl;
	*/
	LLog("server type: %d %d\n", server_map[(uv_tcp_t*)msg->sock->handler]->socktype, server_map[(uv_tcp_t*)msg->sock->handler]->port);


	delete msg->sock;

	uv_rwlock_wrunlock(&id_client_map_rwlock);
}