/*
	日志宏
	用来做库时，一般不给用户用。 用户自己定义新的宏来定义新的实现
*/
#pragma once
#include <string>
#include "log_file.h"

#define L_FATAL(x, ...)  LogMgr::Obj().Printf(LL_FATAL, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_ERROR(x, ...)  LogMgr::Obj().Printf(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_WARN(x, ...)   LogMgr::Obj().Printf(LL_WARN, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_DEBUG(x, ...)  LogMgr::Obj().Printf(LL_DEBUG, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_INFO(x, ...)   LogMgr::Obj().Printf(LL_INFO, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);

//简化if语句写法
#define L_COND(cond, ret, x, ...)\
	do{\
	if(!(cond)){\
	LogMgr::Obj().Printf(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__); \
	return ret;\
	}	\
	}while(0)

#define L_COND_VOID(cond, x, ...)\
	do{\
	if(!(cond))	\
	{\
	LogMgr::Obj().Printf(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__); \
		return; \
	}\
	}while(0)

#define COND(cond, ret)\
	do{\
	if(!(cond)){\
	return ret;\
	}	\
	}while(0)

#define COND_VOID(cond)\
	do{\
	if(!(cond))	\
	return; \
	}while(0)

#define L_ASSERT(cond)\
	do{\
	if(!(cond)){\
	LogMgr::Obj().Printf(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "assert error"); \
	}	\
	}while(0)



