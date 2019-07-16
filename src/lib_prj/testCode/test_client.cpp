
//#include "my_socket.h"
#include "../utility/circleBuf.h"
#include "../utility/log_file.h"
#include <string>

void test_client();

using namespace std;
namespace
{
	const char *ip = "192.168.1.166";
	const uint16 PORT = 5551;
	typedef CircleBuf<uint8, 5000> MyBuf;

}//namespace

void test_client()
{
	//LOG_CONDIT_VOID(Socket::initWinSocket(), "fail");
	//SOCKET socket;
	//SockAddr addr(ip, PORT);

	//LOG_CONDIT_VOID(Socket::connect(socket, addr), "fail connect, ip=%s,port=%d", ip, PORT);

	//MyBuf buf;

	//while (1)
	//{
	//	string s;
	//	cin >> s;
	//	ASSERT_DEBUG(buf.write((const uint8 *)s.c_str(), s.length()));
	//	int ret = EasyRW::send(socket, buf);
 //       if (ret <= 0)
 //       {
 //           cout << "send error, ret = " << ret << endl; 
 //           //for (;;)
 //           //{
 //           //    if(Socket::connect(socket, addr))
 //           //    {
 //           //        cout << "ret connect ok"<< endl; 
 //           //        break;
 //           //    }
 //           //    sleep(1000*1);
 //           //}
 //           return;
 //       }
 //       
	//	if ("exit" == s)
	//	{
	//		::closesocket(socket);
	//		return;
	//	}
	//	if ("no close exit" == s)
	//	{
	//		return;
	//	}
	//	if ("shdown" == s)
	//	{
 //           ::shutdown(socket, SD_BOTH);
	//		return;
	//	}
	//}

}