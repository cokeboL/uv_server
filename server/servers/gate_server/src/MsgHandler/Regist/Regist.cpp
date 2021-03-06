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
	case SOCKTYPE_DISPATCHLOGSERVER:
		extern ServerSock *gDispatchLogSock;
		gDispatchLogSock = New ServerSock(*msg->sock, socktype, IP_DISPATCHLOGSERVER, PORT_DISPATCHLOGSERVER);
		server_map[(uv_tcp_t*)msg->sock->handler] = gDispatchLogSock;
		break;
	case SOCKTYPE_DATASERVER:
		extern ServerSock *gDataSock;
		gDataSock = New ServerSock(*msg->sock, socktype, IP_DATASERVER, PORT_DATASERVER);
		server_map[(uv_tcp_t*)msg->sock->handler] = gDataSock;
		break;
	case SOCKTYPE_BILLSERVER:
		extern ServerSock *gBillSock;
		gBillSock = New ServerSock(*msg->sock, socktype, IP_BILLSERVER, PORT_BILLSERVER);
		server_map[(uv_tcp_t*)msg->sock->handler] = gBillSock;
		break;
	case SOCKTYPE_LOGICSERVER:
		extern std::vector<ServerSock*> gLogicSocks;
		gLogicSocks.push_back(New ServerSock(*msg->sock, socktype, std::string(msg->msg+8), *(int*)(msg->msg+4)));
		server_map[(uv_tcp_t*)msg->sock->handler] = gLogicSocks.at(gLogicSocks.size()-1);
		break;
	default:
		break;
	}
	
	LLog("other server type: %d %d\n", server_map[(uv_tcp_t*)msg->sock->handler]->socktype, server_map[(uv_tcp_t*)msg->sock->handler]->port);
}