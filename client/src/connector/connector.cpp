#include "connector/connector.h"
#include "log/log.h"
#include "rpc/rpc.h"
#include "handler/handler.h"
#include <vector>

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

ServerSock *gGateSock;
ServerSock *gDataSock;
ServerSock *gBillSock;
std::vector<ServerSock*> gLogicSocks;

static uv_tcp_t connector_server;
static uv_tcp_t connector_connect;

//lock
uv_mutex_t sock_id_mutex;
uv_rwlock_t id_client_map_rwlock;

//map
std::unordered_map<unsigned int, Sock*> id_map;
std::unordered_map<uv_tcp_t*, Sock*> client_map;
std::unordered_map<uv_tcp_t*, ServerSock*> server_map;

static void regist_to_server(const char *ip, const int port);

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

static void on_connect_server(uv_connect_t* req, int status)
{	
	if(status == 0)
	{
		uv_read_start((uv_stream_t*)req->handle, alloc_buf, read_data);

		uv_tcp_t *client = (uv_tcp_t*)req->handle;
		ServerSock* sock = new ServerSock(get_sock_id(), client, SOCKTYPE_GATESERVER, std::string("127.0.0.1"), 7401);
		uv_rwlock_wrlock(&id_client_map_rwlock);
		server_map[client] = sock;
		uv_rwlock_wrunlock(&id_client_map_rwlock);
	
		gGateSock = sock;
		//SESSION_KEY
		SESSION_KEY.c_str();
		int packLen = 4;
		int msg = SOCKTYPE_DISPATCHLOGSERVER;
		//SockMsg *registMsg = new SockMsg(sock, CMD_CLIENTREGIST, 0, SESSION_KEY.c_str(), SESSION_KEY.size());
		//rpc_send_msg(registMsg);

		LLog("regist to gate_server OK!\n");
	}
	else
	{
		uv_sleep(100);
		uv_close((uv_handle_t*)&connector_connect, 0);
		regist_to_server(IP_GATESERVER.c_str(), 7401);
	}
	
}

static void regist_to_server(const char *ip, const int port)
{
	static uv_connect_t conn_gate_req;
	uv_tcp_init(uv_default_loop(), &connector_connect);
	uv_tcp_connect(&conn_gate_req, (uv_tcp_t*)&connector_connect, uv_ip4_addr(ip, port), on_connect_server);
}


void start_connector()
{
	uv_mutex_init(&sock_id_mutex);
	uv_rwlock_init(&id_client_map_rwlock);

	//regist_to_server(IP_GATESERVER.c_str(), PORT_GATESERVER);
	regist_to_server("192.168.10.152", 7401);
	
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
