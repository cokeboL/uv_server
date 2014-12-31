#include "connector/connector.h"
#include "log/log.h"
#include "rpc/rpc.h"
#include "handler/handler.h"

static uv_tcp_t connector_server;
static uv_tcp_t connector_connect;

//lock
uv_mutex_t sock_id_mutex;
uv_rwlock_t id_client_map_rwlock;

//map
std::unordered_map<unsigned int, Sock*> id_map;
std::unordered_map<uv_tcp_t*, Sock*> client_map;

static int get_sock_id()
{
	static unsigned int sock_id = 0;

	uv_mutex_lock(&sock_id_mutex);
	sock_id++;
	uv_mutex_unlock(&sock_id_mutex);

	return sock_id;
}




void on_new_connection(uv_stream_t *server, int status)
{
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

        uv_read_start((uv_stream_t*)client, alloc_buf, read_data);
    }
    else {
        uv_close((uv_handle_t*)client, 0);
    }
}


void start_listene(int port)
{
	uv_tcp_init(uv_default_loop(), &connector_server);
	uv_tcp_bind(&connector_server, uv_ip4_addr("127.0.0.1", PORTTYPE_GATESERVER));
	uv_listen((uv_stream_t*)&connector_server, 12, on_new_connection);

}

void start_connector(int port)
{
	uv_mutex_init(&sock_id_mutex);
	uv_rwlock_init(&id_client_map_rwlock);

	start_listene(port);

	//regist_to_gate();
	
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	
	return;
}

void close_connector()
{
	uv_mutex_destroy(&sock_id_mutex);
	uv_rwlock_destroy(&id_client_map_rwlock);

	id_map.clear();
	client_map.clear();

	uv_close((uv_handle_t*)&connector_server, 0);
	uv_close((uv_handle_t*)&connector_connect, 0);
}
