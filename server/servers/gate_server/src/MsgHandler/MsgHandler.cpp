#include "MsgHandler/MsgHandler.h"
#include "MsgHandler/Regist/Regist.h"
#include "MsgHandler/LoadBalance/LoadBalance.h"

void MsgHandler::HandleMsg(SOCKMSG* msg)
{
	switch(msg->cmd){
	case CMD_REGIST:
		Regist::HandleMsg(msg);
		break;
	case CMD_INIT:

		break;
	case CMD_LOG:
		break;
	case CMD_CLIENTREGIST:
		LoadBalance::HandleMsg(msg);
		break;
	default:
		break;
	}
	//std::cout << "id: " << msg->sock->id << msg->cmd << msg->action << " msg: " << msg->msg << std::endl;
}

