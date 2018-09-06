

#include <iostream>
#include "../utility/easyTest.h"
#include "../boostCode/utility/string_tool.h"

#include <boost\pool\object_pool.hpp>
#include <boost\pool\pool_alloc.hpp>
#include <boost/timer/timer.hpp>
#include <windows.h>
#include  <boost/core/noncopyable.hpp>
#include  <boost/thread/mutex.hpp>
#include  <boost/thread/thread.hpp>
#include  <boost/format.hpp>

using namespace boost;
using namespace std;

namespace
{

	void test1()
	{
		object_pool<int> int_pool;

		int *p = int_pool.malloc();
		int_pool.free(p);
		//////////////////////////////////////////////////////////////////////////
		
		{
			std::vector<int, boost::pool_allocator<int> > v;
			for (int i = 0; i < 10000; ++i)
				v.push_back(13);
		} // Exiting the function does NOT free the system memory allocated by the pool allocator.
		// You must call
		//  boost::singleton_pool<boost::pool_allocator_tag, sizeof(int)>::release_memory();
		// in order to force freeing the system memory.

		//////////////////////////////////////////////////////////////////////////
		{
			boost::timer::auto_cpu_timer t;
			for (long i = 0; i < 3000000; ++i)
				int C = 3; // burn some time
			//Sleep(300);
		}

		//////////////////////////////////////////////////////////////////////////
		{
			class X: private boost::noncopyable
			{
			};
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
	}

}//namespace{



void UseBoostExample()
{
	test1();
}