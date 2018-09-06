
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>


using namespace boost;
using namespace std;


namespace
{
	mutex iomutex;

	void runChild(const int n)
	{
		{
			mutex::scoped_lock lock(iomutex);
			cout << "我是第" << n << "个子线程" << endl;
		}

		{
			mutex::scoped_lock lock(iomutex);
			cout << "进程" << n << "退出" << endl;
		}
	}

	void test1()
	{
		

		std::cout << boost::thread::hardware_concurrency() << std::endl; 
		thread_group group;
		for(int num=0;num<10;num++)
			group.create_thread(std::bind(&runChild,num));
		group.join_all();

	}

}//namespace{



void testThread()
{
	test1();
	//Info("done");
}