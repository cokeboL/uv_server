#ifndef _RPC_H_
#define _RPC_H_

#include "commen.h"

//void rpc_push_msg(SockMsg*);

void rpc_send_msg_without_dele(SockMsg* msg);

void rpc_send_msg(SockMsg* msg);

void rpc_push_recv_msg(SockMsg*);

void start_rpc();

void close_rpc();

#endif // _RPC_H_
