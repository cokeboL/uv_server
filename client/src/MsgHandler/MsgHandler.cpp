#include "MsgHandler/MsgHandler.h"
#include "MsgHandler/RegistAndGetLogicInfo/RegistAndGetLogicInfo.h"
#include "MsgHandler/Notify/Notify.h"

void MsgHandler::HandleMsg(SockMsg* msg)
{
	switch(msg->cmd){
	case CMD_CLIENTREGIST:
		RegistAndGetLogicInfo::HandleMsg(msg);
		break;
	case CMD_NOTIFY:
		Notify::HandleMsg(msg);
		break;
	default:
		break;
	}
}

