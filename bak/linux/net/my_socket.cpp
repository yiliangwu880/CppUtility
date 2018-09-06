
#include "stdio.h"
#include "my_socket.h"

bool Socket::connect( int &fd, SockAddr &addr )
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == fd)
	{
		return false;
	}
	int ret = ::connect(fd, (sockaddr *)&addr, sizeof(sockaddr));
	if (-1 == ret)
	{
        ::close(fd);
        fd = -1;
		return false;
	}

	return true;
}

bool Socket::getRemoteAddr(int fd, SockAddr &addr )
{
    socklen_t len = sizeof(addr);
    int ret = getpeername(fd, (struct sockaddr *)&addr, &len);
    if (-1 == ret)
    {
        return false;
    }
    return true;
}

bool Socket::getAddr( int fd, SockAddr &addr )
{
    socklen_t len = sizeof(addr);
    int ret = getsockname(fd, (struct sockaddr *)&addr, &len);
    if (-1 == ret)
    {
        return false;
    }
    return true;
}

void SockListen::setNoblock()
{
    int opt; 

    opt = fcntl(m_fd, F_GETFL); 
    if (opt < 0) { 
        printf("fcntl(F_GETFL) fail."); 
        return;
    } 
    opt |= O_NONBLOCK; 
    if (fcntl(m_fd, F_SETFL, opt) < 0) { 
        printf("fcntl(F_SETFL) fail."); 
        return;
    } 
}

bool SockListen::init( const SockAddr &my_addr, int type /*= SOCK_STREAM */ )
{
    m_fd = socket(AF_INET, type, 0);
    if(-1 == m_fd)
    {
        return false;
    }
    //避免linux 端口不能重用的bug
    int opt = 1; 
    if( 0 != setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        ::close(m_fd);
        m_fd = -1;
        return false;
    }

    int ret = bind(m_fd, (sockaddr *)&my_addr, sizeof(sockaddr));
    if (-1 == ret)
    {
        close(m_fd);
        return false;
    }
    if (-1 == ::listen(m_fd, 10))
    {
        close(m_fd);
        return false;
    }
    return true;
}

bool SockListen::init( const char *ip, uint16 port )
{
    if (NULL == ip)
    {
        return false;
    }
    SockAddr addr(ip, port);
    return init(addr);
}
