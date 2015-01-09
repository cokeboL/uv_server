/*************************************************************************************/
*                              Cimpple means Simple Cpp                              *
/*************************************************************************************/

for windows:
	1.set up vs2012
	2.open project server/server.sln
	3.build all projects
	4.run all exe
	
for linux:
	xxx ...



/**************************************** Doc ****************************************/
Cimpple：
              |---------------------|-----------------------|-----------------------|
              |---------------------|-----------------------|                       |
              |---------------------|                       |                       |
         gate_server 		dispatch_log_server     	data_server    		  	bill_server 
    （负载均衡，防火墙）     （广播，日志等） 		(redis,mongodb/mysql)	（第三方认证，付费等）
              |                     |                       |                       |
	----------------------------------------------------------------------------------------------
              |                     |                       |
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

1.负载均衡（暂时只是实现简单的负载均衡，用户可以根据自己项目情况扩展，比如根据logic server
  当前在线人数取人数少的，或者按连接方IP段按区域的等等。。）
2.连接认证（目前只在配置文件里加了简单session，上线产品往往需要第三方认证机制，具体功能可
  根据项目实际进行实现）
3.广播和单播性能测试，暂时实现是logic_server收到client的所有广播都发给dispatch_log_server
  再转发给所有logic_server发给所有client，当然可以自己给自己内部玩家发，dispatch_log_server
  收到后就不再重新发回来源的logic_server，实现简单，请参考代码自行修改
4.细节处理，如收发包出错，断开连接，认证超时踢掉
5.集成redis
6.集成mongodb或mysql
7.日志系统
8.通信加密
9.跨平台配置（linux、mac os）
10.timer定时器，如连接后应在xx时间内认证，如果超时未认证通过需踢掉玩家，还有心跳超时等
11.命名规范，代码整理

etc.



/**************************************** Other ****************************************/
关于负载均衡，说白了其实都是逻辑上的问题，一下以Cimpple这个基础框架为例简单讨论几种情况：
1.单组服务器，中小型游戏，玩家量不太大，Cimpple开一组server，包括gate、dispatch_log、data、
  bill各一个和logic服务器n个，堆硬件和逻辑服务器进程数，如果需要分区，一个logic服务器就可
  以单独做一个分区
2.多组服务器，既上面1中的多组组合起来，可以web配置下服务器列表，或者干脆再开一个甚至几个
  super gate加一层负载，硬件神马的软件神马的网上堆就是了，至于db_server负载，也可以考虑多
  开几个进程对其他服务器做分流负载，也是设计上的逻辑问题。如果想说一个数据库不够用，请联系
  牛x的dba做数据库集群部署之类的，这个我不懂，就不乱讲了

  
 
/************************************** Contact Me **************************************/
qq：174077440（经常不在线，create issue吧，会收到邮件提醒）
  
	














  
  
  
  
  
  
  