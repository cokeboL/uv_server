#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "commen.h"



void start_listen(int port);

void close_listener();

Sock *get_id_map(Sock* sock);

#endif // _CONNECTOR_H_