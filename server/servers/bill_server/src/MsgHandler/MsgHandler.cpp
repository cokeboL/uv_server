#include "MsgHandler/MsgHandler.h"
#include "Regist/Regist.h"

void MsgHandler::HandleMsg(SockMsg* msg)
{
	switch(msg->cmd){
	case CMD_REGIST:
		Regist::HandleMsg(msg);
		break;
	case CMD_INIT:

		break;
	case CMD_LOG:
		break;
	default:
		break;
	}
	//std::cout << "id: " << msg->sock->id << msg->cmd << msg->action << " msg: " << msg->msg << std::endl;
}

