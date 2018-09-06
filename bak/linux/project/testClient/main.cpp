
#include <stdio.h>
#include "config.h"
#include "logFile.h"

DebugLog g_logFile("log.log");


void testClient();

int  main(int argc, char** argv)
{
	DEBUG("start run");
	testClient();  
	DEBUG("end run");
	return 0;
}

