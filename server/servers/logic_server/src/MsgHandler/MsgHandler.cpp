#include "MsgHandler/MsgHandler.h"
#include "MsgHandler/Notify/Notify.h"

void MsgHandler::HandleMsg(SockMsg* msg)
{
	switch(msg->cmd){
	case CMD_INIT:

		break;
	case CMD_NOTIFY:
		Notify::HandleMsg(msg);
		break;
	default:
		break;
	}
	//std::cout << "id: " << msg->sock->id << msg->cmd << msg->action << " msg: " << msg->msg << std::endl;
}

