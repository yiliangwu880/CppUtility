/*
brief: 对请求的事件，排队同步执行,只合适单线程,可以递归调用
using example:
	QueueEvent<int,int> obj;
	....
	any time want to post event
	obj.postEvent(1);

	any time event have finish;
	obj.finishedEvent(1);
*/
#pragma once
#include "typedef.h"
#include "noncopyable.h"
#include <queue>


template<typename T, typename Result>
class QueueEvent: public Noncopyable
{
public:
	QueueEvent();
	virtual ~QueueEvent(){};
	void postEvent(const T &event);
	bool finishedEvent(const Result &r);
private:
	virtual void onFinishEvent(const Result &r){};
	virtual void onPostEvent(const T &event){};
	
private:
	enum STATE
	{
		S_IDLE,
		S_DOING_FIRST,
		S_DOING_FIRST_HAVE_QUEUE,	//处理第一个事件中，而已有别的事件排队
		S_DOING_QUEUE,				//处理事件中
	};
	std::queue<T> m_queue_event;
	STATE m_state;  
};

template<typename T, typename Result>
QueueEvent<T, Result>::QueueEvent()
:m_state(S_IDLE)
{
}

template<typename T, typename Result>
bool QueueEvent<T, Result>::finishedEvent( const Result &r )
{
	if (S_IDLE == m_state)
	{
		return false;
	}

	onFinishEvent(r);
	if(S_DOING_FIRST == m_state)
	{	
		m_state = S_IDLE;
	}
	else if (S_DOING_FIRST_HAVE_QUEUE == m_state)
	{
		m_state = S_DOING_QUEUE;
		onPostEvent(m_queue_event.back());
	}
	else if (S_DOING_QUEUE == m_state)
	{
		m_queue_event.pop();
		if (m_queue_event.empty())
		{
			m_state = S_IDLE;
			return true;
		}
		onPostEvent(m_queue_event.back());
	}
	else
	{
		return false;
	}

	return true;
}

template<typename T, typename Result>
void QueueEvent<T, Result>::postEvent( const T &event )
{
	if (S_IDLE == m_state)
	{
		m_state = S_DOING_FIRST;
		onPostEvent(event);
		return;
	}
	else if (S_DOING_FIRST == m_state)
	{
		m_state = S_DOING_FIRST_HAVE_QUEUE;
		m_queue_event.push(event);
	}
	else//S_DOING_FIRST_HAVE_QUEUE S_DOING_QUEUE
	{
		m_queue_event.push(event);
	}
}
