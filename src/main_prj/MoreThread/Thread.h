//////////////////////////////////////////////////////////////////////
//
// Thread.h
//
//
/*
太不好用，不合理了，等改：
没jion
没stop  : pthread_cancel
这个exit 是线程内部调用，不应该public

假设接口：
	start
	stop
	jion
	virtual run

*/




//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
//#include "Type.h"
#pragma once
#ifdef WIN32
#define  __WINDOWS__
#include <Windows.h>
#endif


//当定义此宏时，所有线程将只执行一次后就推出。
//#define _EXEONECE 10

//////////////////////////////////////////////////////////////////////
//
// class Thread
//
// POSIX Thread Class
//
//////////////////////////////////////////////////////////////////////

class Thread 
{

#if defined(__WINDOWS__)
	typedef DWORD		TID ;
	//用来定义玩家各个子模型是否可见 ###
	typedef unsigned __int64 MODEL_PART;
#else
	typedef pthread_t	TID ;
	typedef unsigned long long MODEL_PART;
	typedef const char* LPCSTR;
#endif

//////////////////////////////////////////////////
// constants
//////////////////////////////////////////////////
public :
	
	enum ThreadStatus 
	{
		READY ,		// 当前线程处于准备状态
		RUNNING ,	// 处于运行状态
		EXITING ,	// 线程正在退出
		EXIT		// 已经退出 
	};
	

//////////////////////////////////////////////////
// constructor and destructor
//////////////////////////////////////////////////

public :

	// constructor
	Thread ( ) ;

	// destructor
	virtual ~Thread () ;


//////////////////////////////////////////////////
// public methods
//////////////////////////////////////////////////

public :

	bool start () ;
	
	virtual void stop () ;

	void exit ( void * retval = NULL ) ;

	virtual void run ()=0 ;


//////////////////////////////////////////////////
// 
//////////////////////////////////////////////////
public :
	// get thread identifier
	TID getTID () { return m_TID; }
	
	// get/set thread's status
	ThreadStatus getStatus () { return m_Status; }
	void setStatus ( ThreadStatus status ) { m_Status = status; }
	

//////////////////////////////////////////////////
// data members
//////////////////////////////////////////////////

private :

	// thread identifier variable
	TID m_TID;
	
	// thread status
	ThreadStatus m_Status;

#if defined(__WINDOWS__)
	HANDLE m_hThread ;
#endif

};

extern uint g_QuitThreadCount ;

#if defined(__LINUX__)
void * MyThreadProcess ( void * derivedThread ) ;
#elif defined(__WINDOWS__)
DWORD WINAPI MyThreadProcess(  void* derivedThread ) ;
#endif

