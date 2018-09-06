
#include <stdio.h>
#include "config.h"
#include "logFile.h"

DebugLog g_logFile("log.log");


void testEpoll();
void testMsgQue();
void testBacktrace();

int  main(int argc, char** argv)
{
    testBacktrace();
    return 0;

	LOG_DEBUG("start server run"); g_logFile.flush();
	testEpoll(); 
	LOG_DEBUG("end server run");
	return 0;
}

