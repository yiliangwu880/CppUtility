
#include "../lib_prj/CppProto/HandleProto.h"

using namespace std;
using namespace su;
using namespace proto;

#if 0

namespace
{
	class HandleMsg
	{
		static void insert_cs(ConClass &con, const insert_cs &msg)
		{
			UNIT_INFO("handle insert_cs");
		}
	};
}


UNITTEST(testProto)
{
	ProtoMgr::RegProtoParse(HandleMsg::insert_cs);

	string strMsg;
	{ //构建消息，发送
		insert_cs msg;
		msg.a = 1;
		msg.vecInt = decltype(msg.vecInt){1, 2};
		msg.ride.id = 2;
		msg.ride.ids = decltype(msg.ride.ids){3, 4};
		msg.ride.s = "abc";
		msg.ride.m[1] = 2;
		msg.ride.m[11] = 22;

		strMsg = ProtoMgr::Pack(msg);
	}
	//忽略网络发送，接收
	{
		ConClass con;
		ProtoMgr::Dispach(con, strMsg.c_str(), strMsg.length());
	}
}


#endif