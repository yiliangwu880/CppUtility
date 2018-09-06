
#include <stdio.h>
#include "utility/logFile.h"



void testEpoll();
void testMsgQue();
void testBacktrace();

int  main(int argc, char** argv)
{
    testBacktrace();
    return 0;

	LOG_DEBUG("start server run"); DebugLog::GetDefaultLog().flush();
	testEpoll(); 
	LOG_DEBUG("end server run");
	return 0;
}

