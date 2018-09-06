/*
brief:default config define
include it into you self config.h is ok.
*/

#pragma once

//#define DEBUG_MODE

#define LOG_DEBUG(x, ...) 
#define LOG_ERROR(x, ...) 
#define ASSERT_DEBUG(cond) cond;
#define LOG_ASSERT(cond) cond;

#define CONDIT(cond, ret)\
	do{\
	if(!(cond)){\
	return ret;\
	}	\
	}while(0)

#define CONDIT_VOID(cond)\
	do{\
	if(!(cond)){	\
	return; \
	}}while(0)

#define LOG_CONDIT(cond, ret, x, ...) CONDIT(cond, ret)
#define LOG_CONDIT_VOID(cond, x, ...) CONDIT_VOID(cond)


