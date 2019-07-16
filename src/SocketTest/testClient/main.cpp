
#include <stdio.h>
#include "utility/log_file.h"



void testClient();

int  main(int argc, char** argv)
{
	L_DEBUG("start run");
	testClient();  
	L_DEBUG("end run");
	return 0;
}

