/*
brief:支持多线程访问消息队列
*/


#include "sys/socket.h"
#include "easyTest.h"
#include "../Thread/message_queue.h"
#include "circleBuf.h"
#include "../Thread/Thread.h"
#include "misc.h"
#include <vector>
#include "stdio.h"
#include "../utility/BacktraceInfo.hpp"

using namespace wyl;
using namespace std;

namespace
{
  

}//end namespace

void testBacktrace()
{
    Info("============\testBacktrace start============\n");
	CBacktraceInfo::RegHangUpHandler();
	int *p = 0;
	*p = 3;
    Info("============\testBacktrace done================\n");
}


