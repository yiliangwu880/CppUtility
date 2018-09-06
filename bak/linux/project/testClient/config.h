
#pragma once

#include "logFile.h"
//linux testclient prj use
class DebugLog;
extern DebugLog g_logFile;



#define DEBUG_MODE



#define DEBUG(x, ...)  g_logFile.printf(LOG_LV_DEBUG, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);

#define ERROR(x, ...)  g_logFile.printf(LOG_LV_ERROR, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);

#define LOG_CONDIT(cond, ret, x, ...)\
	do{\
	if(!(cond)){\
	g_logFile.error(__FILE__, __LINE__, x, ##__VA_ARGS__);\
	return ret;\
	}	\
	}while(0)

#define LOG_CONDIT_VOID(cond, x, ...)\
	do{\
	if(!(cond))	\
{\
	g_logFile.error(__FILE__, __LINE__, x, ##__VA_ARGS__);\
	return; \
}\
	}while(0)

#define CONDIT(cond, ret)\
	do{\
	if(!(cond)){\
	return ret;\
	}	\
	}while(0)

#define CONDIT_VOID(cond)\
	do{\
	if(!(cond))	\
	return; \
	}while(0)

#define ASSERT_DEBUG(cond)\
	do{\
	if(!(cond)){\
	g_logFile.error(__FILE__, __LINE__, "assert");\
	cout << __FILE__ << ":" <<  __LINE__ << "assert error" << endl;\
	}	\
	}while(0)

