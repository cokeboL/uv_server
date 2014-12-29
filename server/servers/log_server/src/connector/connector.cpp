#include "connector.h"
#include "log/log.h"
#include "rpc/rpc.h"
#include <unordered_map>

static uv_tcp_t* tcp_server;

//lock
static uv_mutex_t sock_id_mutex;
static uv_rwlock_t id_client_map_rwlock;

//map
static std::unordered_map<unsigned int, Sock*> id_map;
static std::unordered_map<uv_tcp_t*, Sock*> client_map;

static int get_sock_id()
{
	static unsigned int sock_id = 0;

	uv_mutex_lock(&sock_id_mutex);
	sock_id++;
	uv_mutex_unlock(&sock_id_mutex);

	return sock_id;
}

static uv_buf_t alloc_buf(uv_handle_t* handle, size_t suggested_size)
{
	return uv_buf_init((char *)malloc(suggested_size), suggested_size);
}

static void close_cb(uv_handle_t* handle)
{
}

static void read_data(uv_stream_t* handle, ssize_t nread, uv_buf_t buf)
{
	const static int pack_len_bytes = 2;
	const static int pack_head_bytes = 4;

	const static int pack_len_not_readed = 0;
	const static int pack_len_readed = 1;
	const static int pack_msg_readed = 2;

	uv_tcp_t* tcp = (uv_tcp_t*)handle;
	uv_shutdown_t* req = 0;
	Sock *sock = 0;

	uv_rwlock_rdlock(&id_client_map_rwlock);
	sock = client_map[tcp];
	uv_rwlock_rdunlock(&id_client_map_rwlock);
	
	if(sock == 0)
	{
		free(buf.base);
		return;
	}

	if (nread < 0)
	{
		if (buf.base)
		{
			free(buf.base);
		}
		goto RELEASE;
	}

	if(nread == 0)
	{
		free(buf.base);
		return;
	}

	int i = 0;
	unsigned char cmd = 0;
	unsigned char action = 0;
	unsigned int len = 0;
	while(i < nread)
	{
		switch(sock->status){
		case pack_len_not_readed:
			if(sock->len_readed == 0)
			{
				sock->len_total = 0;
			}
			for(;i<nread;)
			{
				i++;
				sock->len_readed++;
				if(sock->len_readed == pack_head_bytes)
				{
					sock->len_total = *(short*)buf.base;	
					cmd = *(unsigned char*)(buf.base+pack_len_bytes);
					action = *(unsigned char*)(buf.base+pack_len_bytes+1);
					if(sock->len_total > (1<<28) )
					{
						free(buf.base);
						goto RELEASE;
					}
					sock->buf = (char*)malloc(sock->len_total - pack_head_bytes);
					sock->status = pack_len_readed;
					//sock->len_readed = 0;
					break;
				}

			}
			/*if (i == nread)
			{
				free(buf.base);
				return;
			}
			*/
		case pack_len_readed:
			len = sock->len_total - sock->len_readed < (nread-i) ? (sock->len_total - sock->len_readed) : (nread-i);
			if (len > 0) {
				memcpy(sock->buf, buf.base + i, len);
				sock->len_readed += len;
				i += len;
			}
			if (sock->len_readed == sock->len_total)
			{
				sock->status = pack_msg_readed;
			}
			/*
			else
			{
				free(buf.base);
				return;
			}
			*/
		case pack_msg_readed:
			int len = sock->len_total - pack_head_bytes + 1;
			char *msg = (char*)malloc(len);
			memcpy(msg, sock->buf, len-1);
			msg[len-1] = 0;
			SOCKMSG *sock_msg = new SOCKMSG(sock, cmd, action, msg, len);

			free(sock->buf);
			sock->buf = 0;

			rpc_push_msg(sock_msg);
			
			sock->status = pack_len_not_readed;
			sock->len_readed = 0;
		}
	}
	free(buf.base);
	return;

RELEASE:
	if(sock)
	{
		uv_rwlock_wrlock(&id_client_map_rwlock);
		id_map.erase(sock->id);
		client_map.erase(sock->handler);
		uv_rwlock_wrunlock(&id_client_map_rwlock);
		
		if(sock->buf)
		{
			free(sock->buf);
		}
		delete sock;
	} 
	uv_close((uv_handle_t*)handle, close_cb);	
}

static void read_cb(uv_stream_t* handle, ssize_t nread, uv_buf_t buf)
{
	read_data(handle, nread, buf);
}

void new_connection(uv_stream_t *server, int status) {
    if (status == -1) {
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(uv_default_loop(), client);

    if (uv_accept(server, (uv_stream_t*) client) == 0) {
		Sock* sock = new Sock(get_sock_id(), client);
		
		uv_rwlock_wrlock(&id_client_map_rwlock);
		id_map[sock->id] = sock;
		client_map[client] = sock;
		uv_rwlock_wrunlock(&id_client_map_rwlock);

        uv_read_start((uv_stream_t*)client, alloc_buf, read_cb);

		printf("---------- new connection id: %u ----------\n", sock->id);
    }
    else {
        uv_close((uv_handle_t*)client, 0);
    }
}

void start_listen(int port)
{
	uv_mutex_init(&sock_id_mutex);
	uv_rwlock_init(&id_client_map_rwlock);

	uv_tcp_t* tcp_server = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(uv_default_loop(), tcp_server);
	
	struct sockaddr_in addr = uv_ip4_addr("127.0.0.1", port);
    
	uv_tcp_bind(tcp_server, addr);

    int r = uv_listen((uv_stream_t*) tcp_server, 128, new_connection);
    if (r) {
		free(tcp_server);
    }

	return;
}

void close_listener()
{
	uv_mutex_destroy(&sock_id_mutex);
	uv_rwlock_destroy(&id_client_map_rwlock);

	id_map.clear();
	client_map.clear();

	uv_close((uv_handle_t*)tcp_server, 0);
	free(tcp_server);
}