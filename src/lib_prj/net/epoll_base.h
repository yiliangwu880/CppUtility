
#include <stdio.h>
#include <sys/epoll.h>
#include "../../svr_util/include/typedef.h"
#include "my_socket.h"

#define EPOLL_ALL_EVENT (EPOLLIN|EPOLLOUT|EPOLLPRI|EPOLLERR|EPOLLHUP)

//et模式， 边缘触发，事件只通知 一次。
template<typename Handler, uint32 MAX_REACT_SIZE=10000>
class EpollEtBase
{
public:
	enum Operator
	{
		ADD = EPOLL_CTL_ADD,
		MOD = EPOLL_CTL_MOD,
		DEL = EPOLL_CTL_DEL,
	};
public:
	EpollEtBase()
		:m_fd(-1)
	{
	}
	~EpollEtBase()
	{
		this->close();
	}

	bool init()
	{
		m_fd = epoll_create(MAX_REACT_SIZE);
		if (-1 == m_fd) 
		{
			return false;
		}
		return true;
	}

	inline bool ctrl(Operator op, int target_fd, epoll_event *p_ev)
	{
		//p_ev->events |= EPOLLET;
		if (ADD == op)
		{
			Socket::setNoblock(target_fd);
		}
		if ( -1 == epoll_ctl(m_fd, op, target_fd, p_ev) ) 
		{
			return false;
		}
		return true;
	}

	void loop()
	{
		epoll_event events[MAX_REACT_SIZE];
		for (;;) 
		{
			m_handler.loop();
			int len = epoll_wait(m_fd, events, MAX_REACT_SIZE, -1);
			if (len == -1 || 0 == len) 
			{
				L_ERROR("logic error");
				return;
			}
			for (int i=0; i<len; ++i)
			{
				bool is_do = false;
				if (events[i].events & EPOLLIN) 
				{ 
					m_handler.handleIn(events[i]);
					is_do = true;
				}
				if(events[i].events & EPOLLOUT)
				{
					m_handler.handleOut(events[i]);
					is_do = true;
				}
				if(events[i].events & EPOLLERR)
				{
					m_handler.handleErr(events[i]);
					is_do = true;
				}
				if(events[i].events & EPOLLHUP)
				{
					m_handler.handleHup(events[i]);
					is_do = true;
				}
				if(events[i].events & EPOLLPRI)
				{
					m_handler.handlePri(events[i]);
					is_do = true;
				}
				if (!is_do)
				{
					L_ERROR("未处理类型，events=%d", events[i].events);
				}
			}
		}
	}

	inline void close()
	{
		if (-1 != m_fd)
		{
			::close(m_fd);
			m_fd = -1;
		}
	}

private:
	int m_fd;
	Handler m_handler;
};

//
//	//可考虑 1线程new内存，写 ,2线程读，加入同步内存，通知线程1 del
//	const uint32 MAX_MESSAGE_LEN = 100;
//	class ConcreteEpoll: public Epoll
//	{
//
//	private:
//
//	};
//
////可考虑 1线程new内存，写 ,2线程读，加入同步内存，通知线程1 del
//const uint32 MAX_MESSAGE_LEN = 100;
//class ConcreteEpoll: public Epoll
//{
//
//private:
//
//};


