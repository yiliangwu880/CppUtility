
#include "../lib_prj/CppProto/HandleProto.h"
#include <unordered_map>

using namespace std;
using namespace su;
using namespace proto;


namespace
{

	class HandleMsg
	{
	public:
		static void insert_cs(ConClass &con, const insert_cs &msg_)
		{
			UNIT_INFO("handle insert_cs");
			proto::insert_cs &msg = *(proto::insert_cs*)(&msg_);

			UNIT_ASSERT(msg.ride.ids.size() == 2);
			UNIT_ASSERT(msg.ride.m.size() == 2);

			UNIT_ASSERT(msg.a == 1);
			auto vecInt = vector<uint32_t>{1, 2};
			UNIT_ASSERT(msg.vecInt == vecInt);
			auto ids = vector<uint32_t>{3, 4};
			UNIT_ASSERT(msg.ride.ids == ids);
			UNIT_ASSERT(msg.ride.s == "abc");
			UNIT_ASSERT(msg.ride.m[1] == 2);
			UNIT_ASSERT(msg.ride.m[11] == 22);

		}
	};
}


UNITTEST(testProto)
{
	ProtoMgr::Ins().RegProtoParse(HandleMsg::insert_cs);

	string strMsg;
	{ //构建消息，发送
		insert_cs msg;
		msg.a = 1;
		msg.vecInt = vector<uint32_t>{1, 2};
		msg.ride.ids = vector<uint32_t>{3, 4};
		msg.ride.s = "abc";
		msg.ride.m[1] = 2;
		msg.ride.m[11] = 22;

		strMsg = ProtoMgr::Ins().Pack(msg);
	}
	//模拟接收网络字节流，处理消息
	{
		ConClass con;
		ProtoMgr::Ins().Dispatch(con, strMsg.c_str(), strMsg.length());
	}
}


