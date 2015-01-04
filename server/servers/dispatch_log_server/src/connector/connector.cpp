#include "connector/connector.h"
#include "log/log.h"
#include "rpc/rpc.h"
#include "handler/handler.h"

int PORT_BILLSERVER=0;
int	PORT_DATASERVER=0;
int PORT_GATESERVER=0;
int PORT_DISPATCHLOGSERVER=0;
int PORT_LOGICSERVER=0;

std::string IP_GATESERVER;
std::string IP_DISPATCHLOGSERVER;
std::string IP_DATASERVER;
std::string IP_BILLSERVER;
std::string IP_LOGICSERVER;

static uv_tcp_t connector_server;
static uv_tcp_t connector_connect;

//lock
uv_mutex_t sock_id_mutex;
uv_rwlock_t id_client_map_rwlock;

//map
std::unordered_map<unsigned int, Sock*> id_map;
std::unordered_map<uv_tcp_t*, Sock*> client_map;
std::unordered_map<uv_tcp_t*, Sock*> server_map;

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

void on_connect_other_server(uv_connect_t* req, int status)
{
	printf("-- client on_connect status: %d!\n", status);
	
	
	if(status == 0)
	{
		//server_map[(uv_tcp_t*)req->handle] = new Sock(get_sock_id(), (uv_tcp_t*)req->handle);
		//server_map[(uv_tcp_t*)req->handle].servertype = SOCKTYPE_GATESERVER;
		
		uv_tcp_t *client = (uv_tcp_t*)req->handle;
		Sock* sock = new Sock(get_sock_id(), client);
		sock->socktype = SOCKTYPE_GATESERVER;
		uv_rwlock_wrlock(&id_client_map_rwlock);
		id_map[sock->id] = sock;
		client_map[client] = sock;
		server_map[client] = sock;
		uv_rwlock_wrunlock(&id_client_map_rwlock);

		uv_read_start((uv_stream_t*)req->handle, alloc_buf, read_data);
	
		int packLen = 8;
		char *send_buf = (char*)malloc(packLen);
		*(short*)send_buf = packLen;
		*(char*)(send_buf+2) = (char)CMD_REGIST;
		*(char*)(send_buf+3) = 0;
		*(int*)(send_buf+4) = (int)SOCKTYPE_DISPATCHLOGSERVER;

		uv_buf_t uv_buf;
		uv_buf.base = send_buf;
		uv_buf.len = packLen;

		uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
		write_req->data = send_buf;

		if(uv_write(write_req, (uv_stream_t*)&connector_connect, &uv_buf, 1, send_handler))
		{
			free(send_buf);
			free(write_req);
		}
		
	}
	else
	{
		uv_sleep(100);
		uv_close((uv_handle_t*)&connector_connect, 0);
		regist_to_other_server(IP_GATESERVER.c_str(), PORT_GATESERVER);
	}
	
}

void regist_to_other_server(const char *ip, const int port)
{
	std::cout << "connect: " << ip << ":" << port << std::endl;
	static uv_connect_t conn_gate_req;
	uv_tcp_init(uv_default_loop(), &connector_connect);
	uv_tcp_connect(&conn_gate_req, (uv_tcp_t*)&connector_connect, uv_ip4_addr(ip, port), on_connect_other_server);
}

void start_listene(int port)
{
	uv_tcp_init(uv_default_loop(), &connector_server);
	uv_tcp_bind(&connector_server, uv_ip4_addr(IP_DISPATCHLOGSERVER.c_str(), PORT_DISPATCHLOGSERVER));
	uv_listen((uv_stream_t*)&connector_server, 12, on_new_connection);

}

void start_connector(int port)
{
	uv_mutex_init(&sock_id_mutex);
	uv_rwlock_init(&id_client_map_rwlock);

	start_listene(port);

	regist_to_other_server(IP_GATESERVER.c_str(), PORT_GATESERVER);
	
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
