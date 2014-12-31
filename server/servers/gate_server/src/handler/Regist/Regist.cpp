#include "commen.h"
#include "Regist.h"
#include "connector/connector.h"

/*
static Regist _instance;

Regist *Regist::getInstance()
{
	return &_instance;
}
*/

void Regist::handle_msg(SOCKMSG* msg)
{
	msg->sock->socktype = *(SOCKTYPE*)msg->msg;
	
	server_map[(uv_tcp_t*)msg->sock->handler] = msg->sock;

	std::cout << "server type: " << msg->sock->socktype << std::endl;
}