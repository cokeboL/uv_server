#include "MsgHandler/Notify/Notify.h"
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

void Notify::HandleMsg(SockMsg* msg)
{
	int action = (SOCKTYPE)msg->action;
	std::unordered_map<uv_tcp_t*, ServerSock*>::iterator it;

	SockMsg *msg2 = New SockMsg(*msg);
	switch(action)
	{
	case NOTIFY_CHAT:
	case NOTIFY_MAIL:
		for(it = server_map.begin(); it != server_map.end(); it++)
		{
			if(SOCKTYPE_LOGICSERVER == it->second->socktype)
			{
				msg2->sock = it->second;
				rpc_send_msg_without_dele(msg2);
			}
		}
		delete msg2;

		break;
	default:
		break;
	}
}