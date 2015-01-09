#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "commen.h"
#include <unordered_map>

extern int PORT_BILLSERVER;
extern int PORT_DATASERVER;
extern int PORT_GATESERVER;
extern int PORT_DISPATCHLOGSERVER;
extern int PORT_LOGICSERVER;
extern int PORT_REDIS;

extern std::string IP_GATESERVER;
extern std::string IP_DISPATCHLOGSERVER;
extern std::string IP_DATASERVER;
extern std::string IP_BILLSERVER;
extern std::string IP_LOGICSERVER;
extern std::string IP_REDIS;

extern uv_mutex_t sock_id_mutex;
extern uv_rwlock_t id_client_map_rwlock;

extern ServerSock *gGateSock;
extern ServerSock *gDispatchLogSock;
extern ServerSock *gDataSock;
extern ServerSock *gBillSock;


extern std::unordered_map<unsigned int, Sock*> id_map;
extern std::unordered_map<uv_tcp_t*, Sock*> client_map;
extern std::unordered_map<uv_tcp_t*, ServerSock*> server_map;

void start_connector(const char *ip, const int port);

void close_connector();

#endif // _CONNECTOR_H_