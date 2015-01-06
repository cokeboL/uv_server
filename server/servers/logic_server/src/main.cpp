#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
#include "handler/handler.h"

int main()
{	
	getServersInfo();
	
	start_rpc();
	start_connector(IP_LOGICSERVER.c_str(), PORT_LOGICSERVER);
	
	close_rpc();
	close_connector();

    return 0;
}
