//////////////////////////////////////////////////////////////////////
//
// Thread.h
//
//
/*
̫�����ã��������ˣ��ȸģ�
ûjion
ûstop  : pthread_cancel
���exit ���߳��ڲ����ã���Ӧ��public

����ӿڣ�
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


//������˺�ʱ�������߳̽�ִֻ��һ�κ���Ƴ���
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
	//����������Ҹ�����ģ���Ƿ�ɼ� ###
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
		READY ,		// ��ǰ�̴߳���׼��״̬
		RUNNING ,	// ��������״̬
		EXITING ,	// �߳������˳�
		EXIT		// �Ѿ��˳� 
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

