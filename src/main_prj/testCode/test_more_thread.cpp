
#include "../MoreThread/Thread.h"
#include "../utility/easyTest.h"


namespace
{

	class Test
	{
	public:
		Test(){
			sleep(100);
			Info("r");
		}
	};
	class MyThread : public Thread
	{
	public:
		virtual void run()
		{
			Info("r");
		}
	};

	void ThreadStart(  void* derivedThread ) 
	{
		Thread *p = (Thread *)derivedThread;

			p->run();
		while(1);
	}

}

void testMoreThread()
{
	MyThread a;
	a.start();
	//::CreateThread( NULL, 0, ThreadStart , &a, 0,  NULL ) ;
	Info("create thread end");
}