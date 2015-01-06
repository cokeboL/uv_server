#include "MsgHandler/Logger/Logger.h"

/*
static Logger _instance;

Logger *Logger::getInstance()
{
	return &_instance;
}
*/

void Logger::HandleMsg(SOCKMSG* msg)
{
	record(msg);
}

void Logger::record(SOCKMSG* msg)
{
	std::cout << "id: " << msg->sock->id << " " << msg->cmd << " " << msg->action << " msg: " << msg->msg << std::endl;
}