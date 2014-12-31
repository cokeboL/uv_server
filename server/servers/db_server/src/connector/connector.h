#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "commen.h"
#include <unordered_map>


extern uv_mutex_t sock_id_mutex;
extern uv_rwlock_t id_client_map_rwlock;

extern std::unordered_map<unsigned int, Sock*> id_map;
extern std::unordered_map<uv_tcp_t*, Sock*> client_map;
extern std::unordered_map<uv_tcp_t*, Sock*> server_map;

//extern uv_tcp_t connector_server;
//extern uv_tcp_t connector_connect;


void on_new_connection(uv_stream_t *server, int status);

void on_connect_gate(uv_connect_t* req, int status);

void start_connector(int port);

void close_connector();



#endif // _CONNECTOR_H_