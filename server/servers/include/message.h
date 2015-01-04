#ifndef _MESSAGE_H_
#define _MESSAGE_H_

typedef enum{
	CMD_NONE,
	
	CMD_REGIST,
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

#endif // _MESSAGE_H_
