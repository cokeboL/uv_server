#include "Regist.h"

static Regist _instance;

Regist *Regist::getInstance()
{
	return &_instance;
}

void Regist::handle_msg(SOCKMSG* msg)
{
	msg->sock->socktype = *(SOCKTYPE*)msg->msg;
	std::cout << "server type: " << msg->sock->socktype << std::endl;
}