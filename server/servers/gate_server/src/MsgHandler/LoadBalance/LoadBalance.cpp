#include "MsgHandler/LoadBalance/LoadBalance.h"
#include "handler/handler.h"
#include "connector/connector.h"
#include "rpc/rpc.h"

void LoadBalance::HandleMsg(SOCKMSG* msg)
{
	SimpleBalance(msg);
}

void LoadBalance::SimpleBalance(SOCKMSG* msg)
{
	static unsigned char balanceIdx = 0;
	if(gLogicSocks.size() <= 0)
	{
		SOCKMSG *msg = new SOCKMSG(msg->sock, CMD_CLIENTREGIST, 0, 0, 0);
		rpc_send_msg(msg);
		return;
	}
	if(strncmp(msg->msg, SESSION_KEY.c_str(), msg->len) == 0)
	{
		if(balanceIdx >= gLogicSocks.size())
		{
			balanceIdx = 0;
		}
		
		char buf[20] = {0};
		
		ServerSock *sock = gLogicSocks.at(balanceIdx);
		*(int*)buf = sock->port;
		strncpy(buf+4, sock->ip.c_str(), sock->ip.size());
		
		int len = gLogicSocks.at(balanceIdx)->ip.size()+5;
		
		SOCKMSG *msgOut = new SOCKMSG(msg->sock, CMD_CLIENTREGIST, 0, buf, len);
		rpc_send_msg(msgOut);

		balanceIdx++;
	}
}