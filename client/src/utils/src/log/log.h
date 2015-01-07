#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <string.h>
#include "commen.h"
#include "handler/handler.h"

//local log
#define LLog(fmt, ...) printf(fmt, ##__VA_ARGS__)


#endif // _LOG_H_
