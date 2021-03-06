#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "commen.h"

uv_buf_t alloc_buf(uv_handle_t* handle, size_t suggested_size);

void read_data(uv_stream_t* handle, ssize_t nread, uv_buf_t buf);

void send_handler(uv_write_t* req, int status);

void close_cb(uv_handle_t* handle);

void getServersInfo();

extern std::string SESSION_KEY;

#endif // _HANDLER_H_
