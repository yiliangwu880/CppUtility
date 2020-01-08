
#include <stdio.h>
#include "utility/log_def.h"



void testEpoll();
void testMsgQue();
void testBacktrace();

int  main(int argc, char** argv)
{
  //  testBacktrace();
   // return 0;

	L_DEBUG("start server run"); 
	testEpoll(); 
	L_DEBUG("end server run");
	return 0;
}

