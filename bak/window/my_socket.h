
#pragma once

#include "Winsock2.h"
#include "stdio.h"
#include "stdlib.h"
#include "../utility/typedef.h"



class SockStream;
struct SockAddr;
/*
分析
listen 类
socket stream class: only use read write. create by accept or connect.
connect class: use for connect ,create socket stream(and make self invalid). 

*/
class Socket
{
public:
	static bool initWinSocket();
	static void closeWinSocket();
	static bool connect( SOCKET &fd, SockAddr &addr );

	static void setNoblock(int fd)
	{
		//if (-1 == fd)
		//{
		//	return;
		//}
		//int opt = fcntl(fd, F_GETFL); 
		//if (opt < 0) { 
		//	LOG_ERROR("error logic");
		//	return;
		//} 
		//opt |= O_NONBLOCK; 
		//if (fcntl(fd, F_SETFL, opt) < 0) { 
		//	LOG_ERROR("error logic");
		//	return;
		//} 
	}
	static void setBlock();

    static bool getRemoteAddr(SOCKET fd, SockAddr &addr);



};

struct SockAddr
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

	const char *getIpString() const
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


class SockListen
{
public:
	SockListen()
		:m_fd(-1)
	{};

	bool init(SockAddr &my_addr, int type = SOCK_STREAM )
	{
		m_fd = socket(AF_INET, type, 0);
		if(SOCKET_ERROR  == m_fd)
		{
			return false;
		}

		int ret = bind(m_fd, (sockaddr *)&my_addr, sizeof(sockaddr));
		if (SOCKET_ERROR == ret)
		{
			this->close();
			return false;
		}
		if (SOCKET_ERROR == ::listen(m_fd, 10))
		{
			this->close();
			return false;
		}
		return true;
	}
	
	//失败返回SOCKET_ERROR
	int accept()
	{
		SockAddr remote_addr;
		int len=sizeof(remote_addr);
		return ::accept(m_fd, (sockaddr *)&remote_addr, &len);
	}

	void setBlock();
	void setNoblock()
	{
/*		int opt; 

		opt = fcntl(m_fd, F_GETFL); 
		if (opt < 0) { 
			printf("fcntl(F_GETFL) fail."); 
			return;
		} 
		opt |= O_NONBLOCK; 
		if (fcntl(m_fd, F_SETFL, opt) < 0) { 
			printf("fcntl(F_SETFL) fail."); 
			return;
		} */
	}

	void close()
	{
		if (INVALID_SOCKET != m_fd)
		{
			::closesocket(m_fd);
			m_fd = INVALID_SOCKET;
		}
	}
	inline int fd()const{return m_fd;};

private:
	int m_fd;
};



