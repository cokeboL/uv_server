#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
#include "handler/handler.h"
#include "RedisConnector/RedisConnector.h"

int main()
{	
	getServersInfo();
	
	RedisConnector::StartRedis();

	start_rpc();
	start_connector();
	
	close_rpc();
	close_connector();

    return 0;
}
