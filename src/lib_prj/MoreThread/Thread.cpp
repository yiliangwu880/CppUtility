
////////////////////////////////////////////////////////////////////////////////
//
// Thread.cpp
//
//
// Last Updated : 2005.03.22
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Thread.h"


#if defined(__WINDOWS__)

#include <Tlhelp32.h>
#include <time.h>
#include <dbghelp.h>
#include <stdio.h>
#include <tchar.h>

#define	DUMP_SIZE_MAX	8000	//max size of our dump
#define	CALL_TRACE_MAX	((DUMP_SIZE_MAX - 2000) / (MAX_PATH + 40))	//max number of traced calls
#define	NL				_T("\n")	//new line

#endif



////////////////////////////////////////////////////////////////////////////////
//
// constructor
//
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
Thread::Thread ( ) 
{


	m_TID		= 0 ;
	m_Status	= Thread::READY ;

#if defined(__WINDOWS__)
	m_hThread = NULL ;
#endif

}


////////////////////////////////////////////////////////////////////////////////
//
// destructor (virtual)
//
////////////////////////////////////////////////////////////////////////////////
Thread::~Thread () 
{
	exit(NULL);
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
bool Thread::start () 
{ 
		
	if ( m_Status != Thread::READY )
		return false;

#if defined(__LINUX__)
	int ret=0;
	ret = pthread_create( &m_TID, NULL , MyThreadProcess , this );
	if (0!=ret)
	{
		return false;
	}
#elif defined(__WINDOWS__)
	m_hThread = ::CreateThread( NULL, 0, MyThreadProcess , this, 0, &m_TID ) ;
#endif
	return true;
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
void Thread::stop ()
{
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
void Thread::exit( void * retval )
{
	
		#if defined(__LINUX__)
			pthread_exit( retval );
		#elif defined(__WINDOWS__)
			::CloseHandle( m_hThread ) ;
		#endif

}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
#if defined(__LINUX__)
void * MyThreadProcess ( void * derivedThread )
{
__ENTER_FUNCTION

	Thread * thread = (Thread *)derivedThread;
	if( thread==NULL )
		return NULL;
	
	// set thread's status to "RUNNING"
	thread->setStatus(Thread::RUNNING);

	// here - polymorphism used. (derived::run() called.)
	thread->run();
	
	// set thread's status to "EXIT"
	thread->setStatus(Thread::EXIT);
	
	//INT ret = 0;
	//thread->exit(&ret);

	g_thread_lock.Lock() ;
	g_QuitThreadCount++ ;
	g_thread_lock.Unlock() ;

__LEAVE_FUNCTION

	return NULL;	// avoid compiler's warning
}
#elif defined (__WINDOWS__)

DWORD WINAPI MyThreadProcess(  void* derivedThread )
{
	Thread * thread = (Thread *)derivedThread;

	if( thread==NULL )
		return 0;

	// set thread's status to "RUNNING"
	thread->setStatus(Thread::RUNNING);

	// here - polymorphism used. (derived::run() called.)
	thread->run();

	// set thread's status to "EXIT"
	thread->setStatus(Thread::EXIT);

	thread->exit(NULL);



	return 0;	// avoid compiler's warning
}
#endif



