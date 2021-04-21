

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
	const char *ip ="127.0.0.1";
	const uint16_t PORT = 5551;
	
	typedef CircleBuf<uint8, 5000> MyBuf;

}



void testClient()
{
	SockAddr addr(ip, PORT);
	L_DEBUG("1");
	int sock_stream;
	L_DEBUG("2");
	bool r = Socket::connect(sock_stream, addr);
	L_COND_V(r, "连接失败，ip=%s,port=%d", ip, PORT);
	MyBuf buf;

	uint64 last_tm = 0;
	uint64 total_bytes = 0;
	uint64 use_sec = 0;
	while (1)
	{
		string s(1024,'a');
		uint64 cur_tm = time(nullptr);
		if (cur_tm- last_tm>2)
		{
			last_tm = cur_tm;
			use_sec += 2;
			L_DEBUG("bps=%.2f Mb, total_bytes=%.2f Mb", double(total_bytes) / (1024 * 1024* use_sec), double(total_bytes)/(1024*1024));
			//i7 下，cs同机器， 接近 3028.11 Mb, total_bytes=157461.92 Mb  
		}
		L_ASSERT(buf.write((const uint8 *)s.c_str(), s.length()));
		int len = EasyRW::send(sock_stream, buf);
		total_bytes += len;
	}
}
