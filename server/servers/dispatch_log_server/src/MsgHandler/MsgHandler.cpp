#include "MsgHandler/MsgHandler.h"
#include "Regist/Regist.h"
#include "Logger/Logger.h"
#include "Notify/Notify.h"

void MsgHandler::HandleMsg(SOCKMSG* msg)
{
	switch(msg->cmd){
	case CMD_REGIST:
		Regist::HandleMsg(msg);
		break;
	case CMD_INIT:

		break;
	case CMD_LOG:
		Logger::HandleMsg(msg);
		break;
	default:
		break;
	}
	//std::cout << "id: " << msg->sock->id << msg->cmd << msg->action << " msg: " << msg->msg << std::endl;
}

