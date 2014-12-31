#include "log/log.h"

char log_buf[MaxLogLength];

void send_log_handler(uv_write_t* req, int status)
{
	if (status == 0 && req)
	{
		//free(req->data);
		free(req);
	}
}

/*
void Log(int level, char *msg)
{
	do{
		sprintf(log_buf+4, "[gate server %d] file %s, line %d: %s", level, __FILE__, __LINE__, msg);
		int packLen = strlen(log_buf+4)+1+4;
		*(short*)log_buf = packLen;
		*(char*)(log_buf+2) = (char)CMD_LOG;
		*(char*)(log_buf+3) = 0;
		uv_buf_t uv_buf;
		uv_buf.base = log_buf;
		uv_buf.len = packLen;
		uv_write_t* write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
		write_req->data = log_buf;
		if(uv_write(write_req, (uv_stream_t*)gLogSock->handler, &uv_buf, 1, send_log_handler))
		{
			free(write_req);
		}
	}while(0);
}
*/