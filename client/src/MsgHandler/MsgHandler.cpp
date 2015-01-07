#include "MsgHandler/MsgHandler.h"
#include "MsgHandler/RegistAndGetLogicInfo/RegistAndGetLogicInfo.h"

void MsgHandler::HandleMsg(SOCKMSG* msg)
{
	int port = *(int*)msg->msg;
	char ip[16] = {0};
	switch(msg->cmd){
	case CMD_CLIENTREGIST:
		RegistAndGetLogicInfo::HandleMsg(msg);
		break;
	default:
		break;
	}
}

