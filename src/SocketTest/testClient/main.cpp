
#include <stdio.h>
#include "utility/logFile.h"



void testClient();

int  main(int argc, char** argv)
{
	LOG_DEBUG("start run");
	testClient();  
	LOG_DEBUG("end run");
	return 0;
}

