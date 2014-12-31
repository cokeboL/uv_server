#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "commen.h"
#include <unordered_map>


extern uv_mutex_t sock_id_mutex;
extern uv_rwlock_t id_client_map_rwlock;

extern std::unordered_map<unsigned int, Sock*> id_map;
extern std::unordered_map<uv_tcp_t*, Sock*> client_map;


void start_listen(int port);

void close_listener();

#endif // _CONNECTOR_H_