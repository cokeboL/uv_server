#include "MsgHandler/Logger/Logger.h"

/*
static Logger _instance;

Logger *Logger::getInstance()
{
	return &_instance;
}
*/

void Logger::HandleMsg(SockMsg* msg)
{
	record(msg);
}

void Logger::record(SockMsg* msg)
{
	std::cout << "id: " << msg->sock->id << " " << msg->cmd << " " << msg->action << " msg: " << msg->msg << std::endl;
}