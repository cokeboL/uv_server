#include "RedisConnector/RedisConnector.h"
#include "connector/connector.h"
#include "hiredis/hiredis.h"
#include "log/log.h"
#include "hiredis/hiredis.h"

static redisContext *redisClient;

int RedisConnector::StartRedis()
{
	redisClient = redisConnect(IP_REDIS.c_str(), PORT_REDIS);
	if (redisClient != NULL && redisClient->err) {
		LLog("Error: %s\n", redisClient->errstr);
	}
	redisCommand(redisClient, "set Cimpple SimpleCpp");

	return 0;
}