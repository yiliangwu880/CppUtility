

#include <stdio.h>
#include <sys/epoll.h>
#include "my_socket.h"
#include "../utility/circleBuf.h"
#include "misc.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

void testClient();

namespace
{	
	const char *ip ="183.60.255.67";
	const uint16 PORT = 5551;
	
	typedef CircleBuf<uint8, 5000> MyBuf;

}



void testClient()
{
	SockAddr addr(ip, PORT);
	LOG_DEBUG("1");
	int sock_stream;
	LOG_DEBUG("2");
	bool r = Socket::connect(sock_stream, addr);
	LOG_CONDIT_VOID(r, "Á¬½ÓÊ§°Ü£¬ip=%s,port=%d", ip, PORT);
	MyBuf buf;

	while (1)
	{
		string s;
		cin >> s;
		ASSERT_DEBUG(buf.write((const uint8 *)s.c_str(), s.length()));
		EasyRW::send(sock_stream, buf);
	}
}
