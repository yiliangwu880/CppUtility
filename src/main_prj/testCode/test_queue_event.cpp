
#include "../utility/queue_event.h"
#include "../utility/easyTest.h"

using namespace std;
//using namespace wyl;

namespace
{
	class MyEvent : public QueueEvent<int,int>
	{
	public:
		virtual void onFinishEvent(const int &r)
		{
			m_result.push_back(r);
		};
		virtual void onPostEvent(const int &event)
		{
			
		};
		vector<int> m_result;
		int m_event;
	};

	class LoopEvent : public QueueEvent<int,int>
	{
	public:
		virtual void onFinishEvent(const int &r)
		{
			m_result.push_back(r);
			if (1 == r)
			{
				static bool is_done = false;
				if (is_done)
				{
					postEvent(5);
				}
				is_done = true;
				postEvent(2);
			}
			if (2 == r)
			{
				postEvent(3);
			}
			if (3 == r)
			{
				postEvent(1);
			}
//			1,2,3,5
		};
		virtual void onPostEvent(const int &event)
		{

		};
		vector<int> m_result;
		int m_event;
	};
	void testRecurivePostEvent()
	{
		LoopEvent my_event;
		for (uint32 i=0; i<5; ++i)
		{
			my_event.finishedEvent(i);
		}	
		assert(my_event.m_result.empty());
		
		my_event.postEvent(1);

			assert(my_event.finishedEvent(1));
			assert(my_event.finishedEvent(2));
			assert(my_event.finishedEvent(3));
			assert(my_event.finishedEvent(5));
			assert(my_event.m_result[0]==1);
			assert(my_event.m_result[1]==2);
			assert(my_event.m_result[2]==3);
			assert(my_event.m_result[3]==5);
	}

	void test1()
	{
		MyEvent my_event;

		my_event.postEvent(1); //请求别的进程做事件1

		assert(my_event.finishedEvent(1));
		assert(!my_event.finishedEvent(0));

		my_event.postEvent(2);
		my_event.postEvent(3);
		assert(my_event.finishedEvent(2));
		assert(my_event.finishedEvent(3));
	}
	void test2()
	{
		MyEvent my_event;

		my_event.postEvent(1); //请求别的进程做事件1

		assert(my_event.finishedEvent(1));
		assert(!my_event.finishedEvent(0));

		my_event.postEvent(2);
		my_event.postEvent(3);
		my_event.postEvent(4);
		assert(my_event.finishedEvent(2));
		assert(my_event.finishedEvent(3));
		my_event.postEvent(5);
		assert(my_event.finishedEvent(4));
		assert(my_event.finishedEvent(5));

		assert(!my_event.finishedEvent(0));

		for (uint32 i=0; i<5; ++i)
		{
			assert(my_event.m_result[i]== i+1);
		}
	}
}//namespace{


void testQueueEvent()
{
	test1();
	test2();
	testRecurivePostEvent();
	Info("done");
}