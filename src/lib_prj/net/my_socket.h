
#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../../svr_util/include/typedef.h"


class SockAddr
{
public:
    //自动端口，自动获取本机ip
    SockAddr()
    {
        sin_family = AF_INET;
        sin_addr.s_addr = INADDR_ANY;
        sin_port = 0;
        memset((void*)sin_zero, 0, sizeof(sin_zero));
    }
    SockAddr(const char *ip, uint16 port)
    {
        sin_family = AF_INET;
        sin_addr.s_addr = inet_addr(ip);
        sin_port = htons(port);
        memset((void*)sin_zero, 0, sizeof(sin_zero));
    }

    const char *getIpString()
    {
        if (INADDR_ANY == sin_addr.s_addr)
        {
            char name[100];//多大合适没研究

            //获取域名
            if(-1 == gethostname(name, sizeof(name)))
            {
                return NULL;
            }

            hostent *p_host = gethostbyname(name);
            if (p_host == NULL)
            {
                return NULL;
            }
            return inet_ntoa(*((in_addr *)p_host->h_addr));
        }
        else
        {
            return inet_ntoa(sin_addr);
        }
    }

    inline uint16 port(){return sin_port;}



private:
    short   sin_family;
    unsigned short sin_port;
    in_addr sin_addr;
    char    sin_zero[8];
};


/*
分析
listen 类
socket stream class: only use read write. create by accept or connect.
connect class: use for connect ,create socket stream(and make self invalid). 

*/
class Socket
{
public:
	static void setNoblock(int fd)
	{
		if (-1 == fd)
		{
			return;
		}
		int opt = fcntl(fd, F_GETFL); 
		if (opt < 0) { 
			L_ERROR("error logic");
			return;
		} 
		opt |= O_NONBLOCK; 
		if (fcntl(fd, F_SETFL, opt) < 0) { 
			L_ERROR("error logic");
			return;
		} 
	}
	static void setBlock();
	static bool connect(int &fd, SockAddr &addr);
    static bool getRemoteAddr(int fd, SockAddr &addr);
    static bool getAddr(int fd, SockAddr &addr);
};

class SockListen
{
public:
	SockListen()
		:m_fd(-1)
	{};
	~SockListen()
	{
		close(m_fd);
	}

    bool init(const char *ip, uint16 port);
	bool init(const SockAddr &my_addr, int type = SOCK_STREAM );
	
	//失败返回-1, 成功返回新的file script
	int accept()
	{
		return ::accept(m_fd, NULL, NULL);
	}

	void setBlock();
	void setNoblock();
	inline int fd()const{return m_fd;};
private:
	int m_fd;
};


