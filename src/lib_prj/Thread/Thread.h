
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
		READY ,		// ��ǰ�̴߳���׼��״̬
		RUNNING ,	// ��������״̬
		EXITING ,	// �߳������˳�
		EXIT		// �Ѿ��˳� 
	};

public :
	Thread();
	bool start() ;
    long jion();
    //ʵ��ͨ������run�����˳���
	virtual void stop()=0;
	ThreadStatus getStatus() const { return m_state; }
	
private :
    static void * MyThreadProcess( void * derivedThread ) ;
    //�߳�ִ�к����������˳�������߳�
    virtual void *run()=0;

private :
	pthread_t m_id;
	ThreadStatus m_state;
};




