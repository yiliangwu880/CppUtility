/*
brief:各种多线程，进程锁方法
*/

#pragma once

class Mutex
{
public:
	void acquire (ACE_Time_Value *tv)
	{
		// ACE_TRACE ("ACE_Thread_Mutex::acquire");
		return ACE_OS::thread_mutex_lock (&this->lock_, tv);
	}

	 void release (void)
	{
		// ACE_TRACE ("ACE_Thread_Mutex::release");
		return ACE_OS::thread_mutex_unlock (&this->lock_);
	}

private:
};

template <class Lock>
class Guard
{
public:
	ACE_Guard (Lock &l);

protected:

private:
};