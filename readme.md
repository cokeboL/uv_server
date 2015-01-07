/*************************************** Build ***************************************/
for windows:
	1.set up vs2012
	2.open project server/server.sln
	3.build all projects
	4.run all exe
	
for linux:
	xxx ...



/**************************************** Doc ****************************************/

			  |---------------------|-----------------------|-----------------------|
			  |---------------------|-----------------------|						|
			  |---------------------|						|						|
		gate_server 		dispatch_log_server     	data_server    		  	bill_server 
	（负载均衡，防火墙）     （广播，日志等） 		(redis,mongodb/mysql)	（第三方认证，付费等）
			  |					 	| 		   				| 			 		   	|
	----------------------------------------------------------------------------------------------
			  |					    |					    |
		 logic_server 		   logic_server 		   logic_server ...

1.gate_server（网关服务器）
  管理server组：接收所有其他服务器连接和认证
  后台管理(gm)：接收admin客户端连接，执行gm命令
  负载均衡：client连接gate_server，由gate_server和logic_server组实现（如维护每个logic_server在线玩家数，选玩家最少的logic_server给客户端连接）

2.dispatch_log_server（分发/日志服务器）
  连接到gate_server
  接收data_server、bill_server、logic_server连接和认证
  广播及转发：如chat、mail、notify等，收到广播类包后转发给所有logic_server
  日志：接收所有服务器发来的日志信息，按日志级别进行处理（如print、持久化）
  
3.data_server（数据库服务器）
  连接到gate_server、dispatch_log_server
  接收bill_server、logic_server连接和认证
  对所有服务器发来的请求进行处理（如get、set）
  数据缓存：集成redis
  持久化：集成
  
4.bill_server（第三方认证/付费服务器）
  连接到gate_server、dispatch_log_server、data_server
  接收logic_server连接和认证
  负责客户端登陆认证、玩家付费认证和记录等
  
5.logic_server（逻辑服务器）
  连接到gate_server、dispatch_log_server、data_server、bill_server
  实现具体的游戏逻辑



/**************************************** Plan ****************************************/

1.负载均衡
2.连接认证，server 之间使用配置的密钥，配置文件是否加密
3.echo/chat测试，广播和单播性能测试
4.细节处理，如收发包出错，断开连接，认证超时踢掉
5.集成redis
6.集成mongodb或mysql
7.日志系统
8.通信加密
9.跨平台配置（linux、mac os）

etc.

















  
  
  
  
  
  
  