#include <stdio.h>

#include "commen.h"
#include "connector/connector.h"
#include "rpc/rpc.h"
#include <stdlib.h>




uv_timer_t timer;
void tickHandler(uv_timer_t *req, int status) 
{
	
	static unsigned int count = 0;
	count++;
	printf("-- tickHandler count: %d\n", count);
	if(uv_is_active((uv_handle_t*) req))
	{
		printf("------ alive\n");
	}
	else
	{
		printf("------ not alive\n");
	}
	//uv_close((uv_handle_t*) &timer, close_cb);

}

void tickHandler2(uv_timer_t *req, int status) 
{
	
	static unsigned int count = 0;
	count++;
	printf("-- tickHandler2 count: %d\n", count);
	if(uv_is_active((uv_handle_t*) &timer))
	{
		printf("------ alive\n");
	}
	else
	{
		printf("------ not alive\n");
	}


}



void getServersInfo()
{
	FILE *fp = fopen("./config", "r");
	if(!fp)
	{
		return;
	}

	char buf[512];

	while(fgets(buf, sizeof(buf), fp))
	{
		if(strncmp(buf, "gate", strlen("gate")) == 0)
		{
			std::string s = std::string(buf);
			IP_GATESERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_GATESERVER = atoi(s.c_str());
												
		}
		else if(strncmp(buf, "dispatch_log", strlen("dispatch_log")) == 0)
		{
			std::string s = std::string(buf);
			IP_DISPATCHLOGSERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_DISPATCHLOGSERVER = atoi(s.c_str());
		}
		else if(strncmp(buf, "data", strlen("data")) == 0)
		{
			std::string s = std::string(buf);
			IP_DATASERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_DATASERVER = atoi(s.c_str());
		}
		else if(strncmp(buf, "bill", strlen("bill")) == 0)
		{
			std::string s = std::string(buf);
			IP_BILLSERVER = s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1);
			s = s.substr(s.find_last_of(" ")+1);
			PORT_BILLSERVER = atoi(s.c_str());
		}
	}
}

int main()
{	
	getServersInfo();

	start_rpc();
	start_connector(7001);
	
	close_rpc();
	close_connector();

    return 0;
}
