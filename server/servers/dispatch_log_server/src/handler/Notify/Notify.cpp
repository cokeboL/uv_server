#include "handler/Notify/Notify.h"
#include "connector/connector.h"
#include "log/log.h"
#include "rpc/rpc.h"

/*
static Regist _instance;

Regist *Regist::getInstance()
{
	return &_instance;
}
*/

void Notify::handle_msg(SOCKMSG* msg)
{
	int action = *(SOCKTYPE*)msg->action;
	std::unordered_map<uv_tcp_t*, ServerSock*>::iterator it;

	switch(action)
	{
	case NOTIFY_CHAT:
	case NOTIFY_MAIL:
		for(it = server_map.begin(); it != server_map.end(); it++)
		{
			if(SOCKTYPE_LOGICSERVER == it->second->socktype)
			{
				SOCKMSG *msg = new SOCKMSG(*msg);
				msg->sock = it->second;
				rpc_push_msg(msg);
			}
		}
		break;
	default:
		break;
	}
}