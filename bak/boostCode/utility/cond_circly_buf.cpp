
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include "../utility/circleBuf.h"
#include "../utility/misc.h"
#include "../utility/easyTest.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/timer/timer.hpp>

using namespace boost;
using namespace std;

namespace
{
	CircleBuf<uint32, 10> g_buf;
	boost::mutex     g_full_lock;
	boost::condition g_full_cond;
	boost::mutex     g_empty_lock;
	boost::condition g_empty_cond;
	uint32 g_full_cond_cnt=0;
	uint32 g_empty_cond_cnt=0;

	void WriteThread()
	{
		for (uint32 i=0; i<NUM_1000W; ++i)
		{
			if(g_buf.full())
			{
				//wait
				mutex::scoped_lock lock(g_full_lock);
				while (g_buf.full())
				{
					g_full_cond.wait(lock);
				}
				g_full_cond_cnt++;
			}
			assert(g_buf.push(i));

			if (g_buf.len()<=1)//读取线程有可能满
			{
				mutex::scoped_lock lock(g_empty_lock);
				g_empty_cond.notify_one();
			}
		}
	}

	void ReadThread()
	{
		uint32 num=-1;
		for (uint32 i=0; i<NUM_1000W; ++i)
		{
			if (g_buf.empty())
			{
				//wait
				mutex::scoped_lock lock(g_empty_lock);
				while (g_buf.empty())
				{
					g_empty_cond.wait(lock);
				}
				g_empty_cond_cnt++;
			}

			uint32 old = num;
			assert(g_buf.pop(num));
			assert(num == old+1);

			if (g_buf.freeLen()<=1) //写入线程有可能满
			{
				mutex::scoped_lock lock(g_full_lock);
				g_full_cond.notify_one();
			}
		}
	}



	void WriteNoLock()
	{
		for (uint32 i=0; i<NUM_1000W; ++i)
		{
			if(g_buf.full())
			{
				//wait
				--i; continue;
			}
			assert(g_buf.push(i));
		}
	}

	void ReadNoLock()
	{
		uint32 num=-1;
		for (uint32 i=0; i<NUM_1000W; ++i)
		{
			if (g_buf.empty())
			{
				//wait
				--i; continue;
			}

			uint32 old = num;
			assert(g_buf.pop(num));
			assert(num == old+1);
		}
	}
}


void TestCondCirclyBuf()
{
	Info("TestCondCirclyBuf start");
	boost::timer::auto_cpu_timer t;
	thread_group group;
	group.create_thread(WriteThread);
	group.create_thread(ReadThread);
	//group.create_thread(WriteNoLock);
	//group.create_thread(ReadNoLock);
	group.join_all();
	printf("TestCondCirclyBuf done %d %d\n", g_empty_cond_cnt, g_full_cond_cnt);
}

