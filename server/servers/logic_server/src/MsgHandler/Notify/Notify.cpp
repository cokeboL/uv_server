#include "MsgHandler/Notify/Notify.h"
#include "connector/connector.h"
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
	switch(msg->action)
	{
	case NOTIFY_CHAT:
		Chat(msg);
		break;
	default:
		
		break;
	}
}

void Notify::Chat(SockMsg* msg)
{
	SockMsg *msg2 = new SockMsg(*msg);
	//client notify msg
	if(client_map[msg->sock->handler])
	{
		msg2->sock = gDispatchLogSock;
		rpc_send_msg(msg2);
	}
	//server notify msg
	else
	{
		
		std::unordered_map<unsigned int, Sock*>::iterator it;
		for(it = id_map.begin(); it != id_map.end(); it++)
		{
			msg2->sock = it->second;
			rpc_send_msg_without_dele(msg2);
		}
		delete msg2;
	}
}