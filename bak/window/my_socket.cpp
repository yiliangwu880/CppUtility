
#include "my_socket.h"

#pragma comment(lib, "Ws2_32.lib")



bool Socket::initWinSocket()
{
	//初始化socket编程环境
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		return false;
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) 
	{
		WSACleanup( );
		return false; 
	}
	return true;
}

void Socket::closeWinSocket()
{
	WSACleanup(); 
}

bool Socket::connect( SOCKET &fd, SockAddr &addr )
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == fd)
	{
		return false;
	}
	int ret = ::connect(fd, (const struct sockaddr FAR*)&addr, sizeof(SockAddr));
	if (SOCKET_ERROR == ret)
	{
        ::closesocket(fd);
		return false;
	}
	return true;
}

bool Socket::getRemoteAddr( SOCKET fd, SockAddr &addr )
{
    int len = sizeof(addr);
    int ret = getpeername(fd, (struct sockaddr *)&addr, &len);
    if (SOCKET_ERROR  == ret)
    {
        return false;
    }
    return true;
}

