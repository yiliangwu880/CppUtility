
//#include "my_socket.h"
#include "../utility/single_progress.h"


void test_single_progresss()
{
	L_DEBUG("test_single_progresss");
	SingleProgress::Instance().CheckSingle("my_name");

}