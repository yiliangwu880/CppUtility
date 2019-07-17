/*
brief:支持多线程访问消息队列
*/


#include "sys/socket.h"
#include "../Thread/message_queue.h"
#include "../utility/circleBuf.h"
#include "../Thread/Thread.h"
#include "misc.h"
#include <vector>
#include "stdio.h"
#include "../utility/BacktraceInfo.h"

using namespace wyl;
using namespace std;

namespace
{
  

}//end namespace

void testBacktrace()
{
    Info("============\testBacktrace start============\n");
	CBacktraceInfo::Instance().RegHangUpHandler();
	int *p = 0;
	*p = 3;
    Info("============\testBacktrace done================\n");
}


