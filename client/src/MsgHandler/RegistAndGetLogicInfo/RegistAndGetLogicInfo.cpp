#include "MsgHandler/RegistAndGetLogicInfo/RegistAndGetLogicInfo.h"



void RegistAndGetLogicInfo::HandleMsg(SockMsg* msg)
{
	int port = *(int*)msg->msg;
	char ip[16] = {0};
	switch(msg->cmd){
	case CMD_CLIENTREGIST:
		strncpy(ip, msg->msg+4, msg->len-4);
		std::cout << "logic server addr: " << ip << ":" << port << std::endl;
		break;
	case CMD_NOTIFY:
		strncpy(ip, msg->msg+4, msg->len-4);
		std::cout << "logic server addr: " << ip << ":" << port << std::endl;
		break;
	default:
		break;
	}
}

