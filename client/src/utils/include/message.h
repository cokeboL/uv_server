#ifndef _MESSAGE_H_
#define _MESSAGE_H_

typedef enum{
	CMD_NONE,
	
	CMD_REGIST,
	CMD_CLIENTREGIST,
	CMD_INIT,
	
	CMD_LOG,

	CMD_NOTIFY,
	CMD_MAX,
}CMD;

typedef enum{
	REGIST_NONE,

	REGIST_MAX,
}REGIST_ACTION;

typedef enum{
	INIT_NONE,

	INIT_MAX,
}INIT_ACTION;


typedef enum{
	NOTIFY_NONE,

	NOTIFY_CHAT,
	NOTIFY_MAIL,

	NOTIFY_MAX,
}NOTIFY_ACTION;
//load balancing

#endif // _MESSAGE_H_
