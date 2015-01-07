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

void Regist::HandleMsg(SockMsg* msg)
{
	uv_rwlock_wrlock(&id_client_map_rwlock);
	id_map.erase(msg->sock->id);
	client_map.erase((uv_tcp_t*)msg->sock->handler);
	uv_rwlock_wrunlock(&id_client_map_rwlock);
	//id_map[msg->sock->id] = 0;
	//client_map[(uv_tcp_t*)msg->sock->handler] = 0;

	SOCKTYPE socktype = *(SOCKTYPE*)msg->msg;
	switch(socktype)
	{
	case SOCKTYPE_LOGICSERVER:
		extern std::vector<ServerSock*> gLogicSocks;
		gLogicSocks.push_back(new ServerSock(*msg->sock, socktype, std::string(msg->msg+8), *(int*)(msg->msg+4)));
		server_map[(uv_tcp_t*)msg->sock->handler] = gLogicSocks.at(gLogicSocks.size()-1);
		break;
	default:
		break;
	}
	
	LLog("other server type: %d %d\n", server_map[(uv_tcp_t*)msg->sock->handler]->socktype, server_map[(uv_tcp_t*)msg->sock->handler]->port);
}