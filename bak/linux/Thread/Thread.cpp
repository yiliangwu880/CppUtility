
#include "Thread.h"

Thread::Thread( ) 
    :m_id(0)
    ,m_state(READY)
{
}

bool Thread::start() 
{ 
	if ( m_state != Thread::READY )
		return false;
	pthread_create( &m_id, NULL , MyThreadProcess , this );
    return true;
}

void * Thread::MyThreadProcess( void * para )
{
    Thread * thread =(Thread *)para;
    if( thread==NULL )
        return NULL;
    thread->m_state = Thread::RUNNING;
    void *ret = thread->run();
    thread->m_state = Thread::EXIT;
    return ret;	
}

long Thread::jion()
{
    void *ret=0;
    pthread_join(m_id, &ret);
    return (long)ret;
}

