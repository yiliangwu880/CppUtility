
#pragma once

#include "stddef.h"
#include <pthread.h>

/*
use example:
class MyThread : public Thread
{
    bool is_stop = false;
    virtual void stop()
    {
        is_stop = true;
    }

    virtual void *run ()
    {
        while (1)
        {
            if (is_stop)
            {
                break;
            }
            do something
        }
        return 0;
    }
}
*/

class Thread 
{
public :
	enum ThreadStatus 
	{
		READY ,		// 当前线程处于准备状态
		RUNNING ,	// 处于运行状态
		EXITING ,	// 线程正在退出
		EXIT		// 已经退出 
	};

public :
	Thread();
	bool start() ;
    long jion();
    //实现通常控制run函数退出。
	virtual void stop()=0;
	ThreadStatus getStatus() const { return m_state; }
	
private :
    static void * MyThreadProcess( void * derivedThread ) ;
    //线程执行函数，函数退出会结束线程
    virtual void *run()=0;

private :
	pthread_t m_id;
	ThreadStatus m_state;
};




