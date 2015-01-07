#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
#include "handler/handler.h"

int main(int argc, char **argv)
{	
	getServersInfo();
	if(argc >= 3)
	{
		IP_LOGICSERVER = std::string(argv[1]);
		PORT_LOGICSERVER = atoi(argv[2]);
		std::cout << "self ip: " << IP_LOGICSERVER.c_str() << " port: " << PORT_LOGICSERVER << std::endl;
	}
	for(int i=0; i<argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}
	
	start_rpc();
	start_connector(IP_LOGICSERVER.c_str(), PORT_LOGICSERVER);
	
	close_rpc();
	close_connector();

    return 0;
}
