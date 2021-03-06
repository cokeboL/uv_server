#include "connector/connector.h"
#include "log/log.h"
#include "rpc/rpc.h"
#include "handler/handler.h"


int PORT_BILLSERVER=0;
int PORT_DATASERVER=0;
int PORT_GATESERVER=0;
int PORT_DISPATCHLOGSERVER=0;
int PORT_LOGICSERVER=0;
int PORT_REDIS = 0;

std::string IP_GATESERVER;
std::string IP_DISPATCHLOGSERVER;
std::string IP_DATASERVER;
std::string IP_BILLSERVER;
std::string IP_LOGICSERVER;
std::string IP_REDIS;

ServerSock *gGateSock;
ServerSock *gDispatchLogSock;
ServerSock *gBillSock;
std::vector<ServerSock*> gLogicSocks;

static uv_tcp_t connector_server;

static uv_tcp_t connector_gate_connect;
static uv_tcp_t connector_dispatch_log_connect;

//lock
uv_mutex_t sock_id_mutex;
uv_rwlock_t id_client_map_rwlock;

//map
std::unordered_map<unsigned int, Sock*> id_map;
std::unordered_map<uv_tcp_t*, Sock*> client_map;
std::unordered_map<uv_tcp_t*, ServerSock*> server_map;

static void regist_to_other_server(const char *ip, const int port);

/*
static int start_redis()
{

	redisContext *c = redisConnect("127.0.0.1", 6379);
	if (c != NULL && c->err) {
		printf("Error: %s\n", c->errstr);
		// handle error
	}
	redisCommand(c, "set gaofeng niubility");
	printf("ALL TESTS PASSED\n");
	return 0;
}
*/

static int get_sock_id()
{
	static unsigned int sock_id = 0;

	uv_mutex_lock(&sock_id_mutex);
	sock_id++;
	uv_mutex_unlock(&sock_id_mutex);

	return sock_id;
}




static void on_new_connection(uv_stream_t *server, int status)
{
    if (status == -1) {
        // error!
        return;
    }

	uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(uv_default_loop(), client);

    if (uv_accept(server, (uv_stream_t*) client) == 0) {
		Sock* sock = New Sock(get_sock_id(), client);
		
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

static void on_connect_other_server(uv_connect_t* req, int status)
{
	if(status == 0)
	{
		uv_read_start((uv_stream_t*)req->handle, alloc_buf, read_data);
	
		uv_tcp_t *client = (uv_tcp_t*)req->handle;
		ServerSock* sock = New ServerSock(get_sock_id(), client, SOCKTYPE_GATESERVER, IP_GATESERVER, PORT_GATESERVER);
		uv_rwlock_wrlock(&id_client_map_rwlock);
		server_map[client] = sock;
		uv_rwlock_wrunlock(&id_client_map_rwlock);
		
		if((void*)&connector_gate_connect == (void*)req->handle)
		{
			gGateSock = sock;
			sock->ip = IP_GATESERVER;
			sock->port = PORT_GATESERVER;
			sock->socktype = SOCKTYPE_GATESERVER;
			LLog("regist to gate_server OK!\n");
		}
		else if((void*)&connector_dispatch_log_connect == (void*)req->handle)
		{
			gDispatchLogSock = sock;
			sock->ip = IP_DISPATCHLOGSERVER;
			sock->port = PORT_DISPATCHLOGSERVER;
			sock->socktype = SOCKTYPE_DISPATCHLOGSERVER;
			LLog("regist to dispatch_log_server OK!\n");
		}
		int packLen = 4;
		int msg = SOCKTYPE_DATASERVER;
		SockMsg *registMsg = New SockMsg(sock, CMD_REGIST, 0, (char*)&msg, packLen);
		rpc_send_msg(registMsg);
	}
	else
	{
		uv_sleep(100);
		if((void*)&connector_gate_connect == (void*)req->handle)
		{
			uv_close((uv_handle_t*)&connector_gate_connect, 0);
			regist_to_other_server(IP_GATESERVER.c_str(), PORT_GATESERVER);
		}
		else if((void*)&connector_dispatch_log_connect == (void*)req->handle)
		{
			uv_close((uv_handle_t*)&connector_dispatch_log_connect, 0);
			regist_to_other_server(IP_DISPATCHLOGSERVER.c_str(), PORT_DISPATCHLOGSERVER);
		}
	}
	free(req);
}

static void regist_to_other_server(const char *ip, const int port)
{
	const int CPORT_GATESERVER = PORT_GATESERVER;
	const int CPORT_DISPATCHLOGSERVER = PORT_DISPATCHLOGSERVER;
	uv_connect_t *conn_gate_req = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	
	if(port == PORT_GATESERVER)
	{
		uv_tcp_init(uv_default_loop(), &connector_gate_connect);
		uv_tcp_connect(conn_gate_req, (uv_tcp_t*)&connector_gate_connect, uv_ip4_addr(ip, port), on_connect_other_server);
	}
	else if(port == PORT_DISPATCHLOGSERVER)
	{
		uv_tcp_init(uv_default_loop(), &connector_dispatch_log_connect);
		uv_tcp_connect(conn_gate_req, (uv_tcp_t*)&connector_dispatch_log_connect, uv_ip4_addr(ip, port), on_connect_other_server);
	}
}

static void start_listene()
{
	uv_tcp_init(uv_default_loop(), &connector_server);
	uv_tcp_bind(&connector_server, uv_ip4_addr(IP_DATASERVER.c_str(), PORT_DATASERVER));
	uv_listen((uv_stream_t*)&connector_server, 12, on_new_connection);
	
	LLog("data_server start at: %s %d\n", IP_DATASERVER.c_str(), PORT_DATASERVER);
}

void start_connector()
{
	uv_mutex_init(&sock_id_mutex);
	uv_rwlock_init(&id_client_map_rwlock);

	regist_to_other_server(IP_GATESERVER.c_str(), PORT_GATESERVER);
	regist_to_other_server(IP_DISPATCHLOGSERVER.c_str(), PORT_DISPATCHLOGSERVER);
	//start_redis();

	start_listene();

	
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
	uv_close((uv_handle_t*)&connector_gate_connect, 0);
	uv_close((uv_handle_t*)&connector_dispatch_log_connect, 0);
}
