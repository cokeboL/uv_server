#ifndef _RPC_H_
#define _RPC_H_

#include "commen.h"

//void rpc_push_msg(SOCKMSG*);

void rpc_send_msg(SOCKMSG* msg);

void rpc_push_recv_msg(SOCKMSG*);

void start_rpc();

void close_rpc();

#endif // _RPC_H_
