
#include <stdio.h>
#include <sys/epoll.h>
#include "epoll_base.h"
#include "../utility/circleBuf.h"
#include "misc.h"
#include <iostream>
#include <map>

using namespace std;
using namespace wyl;


void testEpoll();

namespace
{
	class MyHandler
	{
	public:
		void handleIn(const epoll_event &event);
		void handleOut(const epoll_event &event);
		void handleErr(const epoll_event &event);
		void handleHup(const epoll_event &event);
		void handlePri(const epoll_event &event);
		void loop();
	};
	typedef EpollEtBase<MyHandler> MyEpollBase;
	typedef CircleBuf<uint8, 5000> MyBuf;
	struct MySocket 
	{
		int fd;
		MyBuf recv_buf, write_buf;
	};
	typedef map<int, MySocket> FdMapMySocket;
	

	//const char *ip ="192.168.1.166";
	const uint16 PORT = 5551;
	SockListen g_listen;
	MyEpollBase g_epoll;
	FdMapMySocket g_fd_map_socket;


	void MyHandler::handlePri( const epoll_event &event )
	{
		LOG_ERROR("priority error");
	}

	void MyHandler::handleHup( const epoll_event &event )
	{
		MySocket *p = (MySocket *)event.data.ptr;
		int fd = p->fd;
		LOG_DEBUG("挂起 fd=%d", fd);
		size_t ret = g_fd_map_socket.erase(fd);
		LOG_CONDIT_VOID(1 == ret, "g_fd_map_socket.erase error, fd=%d", fd);
		::close(p->fd);
        p->fd = -1;
	}

	void MyHandler::handleErr( const epoll_event &event )
	{
		MySocket *p = (MySocket *)event.data.ptr;
		int fd = p->fd;
		LOG_DEBUG("fd=%d error", fd);
	}

	void MyHandler::handleOut( const epoll_event &event )
	{
		MySocket *p = (MySocket *)event.data.ptr;
		ASSERT_DEBUG(-1 != p->fd);
		while (1)
		{
			int ret = EasyRW::send(p->fd, p->write_buf);
			if (0 == ret || -1 ==ret )
			{
				break;
			}
		}
	}

	void MyHandler::handleIn( const epoll_event &event )
	{
		MySocket *pMySocket = (MySocket *)event.data.ptr;
		if (pMySocket->fd == g_listen.fd())
		{
			//listen
			bool is_accept = false;
			while(1)
			{
				int fd = g_listen.accept();
				if (-1 == fd)
				{
					break;
				}
				is_accept = true;
				LOG_DEBUG("new connect fd=%d", fd);
				ASSERT_DEBUG(g_fd_map_socket.find(fd) == g_fd_map_socket.end());
				MySocket data;
				data.fd = fd;
				FdMapMySocket::iterator it= g_fd_map_socket.insert(make_pair(fd, data)).first;

				epoll_event ev;
				ev.events = EPOLL_ALL_EVENT;
				ev.data.ptr = (void *)&(it->second);
				ASSERT_DEBUG(g_epoll.ctrl(EpollEtBase<MyHandler>::ADD, fd, &ev));
			}
			if (!is_accept)
			{
				LOG_DEBUG("handleIn listen fd=%d, but accept fail", g_listen.fd());
			}
			
		}
		else
		{
			//read
			ASSERT_DEBUG(-1 != event.data.fd);
			while (1)
			{
				int ret = EasyRW::recv(pMySocket->fd, pMySocket->recv_buf);
				if (0 == ret || -1 == ret)
				{
					break;
				}
				LOG_DEBUG("rev data");
			}
		}
	}

	void MyHandler::loop()
	{
		//有缓存，输出到cout
		FOR_IT(FdMapMySocket, g_fd_map_socket)
		{
			MyBuf &buf = it->second.recv_buf;
			if (0 == buf.len())
			{
				continue;
			}
            {
                unsigned char char_buf[1000];
                bool ret = false;
                uint32 len = 0;
                buf.getCanContinueRead(len);	
                if (0 != len && len < ArrayLen(char_buf))
                {
                   ret = buf.peer(char_buf, len);
                }
                if (ret)
                {
                    char_buf[len]=0;
                    LOG_DEBUG("write data to cout [%s]", char_buf);
                }
                else
                {
                    LOG_DEBUG("write empty data to cout");
                }
            }
			while(1)
			{
				int ret = EasyRW::write(cout, buf);
				if (0 == ret || -1 ==ret )
				{
					break;
				}
				cout << endl;
			}
		}
	}
}



void testEpoll()
{
	daemon(1,1);
	ASSERT_DEBUG(g_epoll.init());
	SockAddr addr("0.0.0.0", PORT);
	if(!g_listen.init(addr))
    {
        LOG_ERROR("init fail");
        return;
    }

	int fd = g_listen.fd();
	MySocket data;
	data.fd = fd;
	FdMapMySocket::iterator it= g_fd_map_socket.insert(make_pair(fd, data)).first;
	epoll_event ev;
	ev.events = EPOLL_ALL_EVENT;
	ev.data.ptr = (void *)&(it->second);
	if(!(g_epoll.ctrl(EpollEtBase<MyHandler>::ADD, fd, &ev)))
    {
        LOG_ERROR("g_epoll.ctrl fail");
        return;
    }

	g_epoll.loop();
}
