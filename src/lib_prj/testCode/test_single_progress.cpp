
//#include "my_socket.h"
#include "../utility/single_progress.hpp"


void test_single_progresss()
{
	LOG_DEBUG("test_single_progresss");
	SingleProgress::Instance().CheckSingle("my_name");

}