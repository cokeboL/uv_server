#include "commen.h"
#include "log/log.h"
#include "rpc/rpc.h"

extern ServerSock *gDispatchLogSock;

void record_log(int level, char *msg)
{
	char msgBuf[MaxLogLength] = {0};

	sprintf(msgBuf, "[gate server %d] file %s, line %d: %s", level, __FILE__, __LINE__, msg);
	int packLen = strlen(msgBuf)+1+4;

	SockMsg *logMsg = New SockMsg(gDispatchLogSock, CMD_LOG, 0, msgBuf, packLen);
	rpc_send_msg(logMsg);
}
