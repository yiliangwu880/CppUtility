
#include "../lib_prj/CppProto/ProtoMgr.h"
#include <unordered_map>

using namespace std;
using namespace su;
using namespace proto;


namespace
{


	//示范用，实际项目可以用更高效的方法，避免内存复制
	template<class MsgType>
	std::string PackToString(const MsgType &msg)
	{
		char ar[1024];
		size_t len = 1024;
		char *p = ar;
		UNIT_ASSERT(proto::Pack(msg, p, len));
		return std::string(ar, 1024 - len);
	}

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

			{
				Data d = Data{ 1,vector<uint32_t>{2,3} };
				UNIT_ASSERT(msg.vecData.size() == 1);
				UNIT_ASSERT(msg.vecData[0].id == d.id);
				UNIT_ASSERT(msg.vecData[0].ids == d.ids);
			}
			{
				Data d1 = Data{ 2,vector<uint32_t>{2,3} };
				Data d2 = Data{ 3,vector<uint32_t>{22,33} };
				UNIT_ASSERT(msg.mapData.size() == 2);
				UNIT_ASSERT(msg.mapData[2].id == d1.id);
				UNIT_ASSERT(msg.mapData[2].ids == d1.ids);
				UNIT_ASSERT(msg.mapData[3].id == d2.id);
				UNIT_ASSERT(msg.mapData[3].ids == d2.ids);

			}
		}

		static void insert_sc(ConClass &con, const insert_sc &msg_)
		{
			UNIT_INFO("handle insert_sc");
			proto::insert_sc &msg = *(proto::insert_sc*)(&msg_);
			UNIT_ASSERT(msg.ret == true);
			UNIT_ASSERT(msg.a == 6);
		}
	};
}


UNITTEST(testProto)
{
	ProtoMgr::Ins().RegMsgHandler(HandleMsg::insert_cs);
	ProtoMgr::Ins().RegMsgHandler(HandleMsg::insert_sc);

	string strMsg;
	{ //构建消息，发送
		insert_cs msg;
		msg.a = 1;
		msg.vecInt = vector<uint32_t>{1, 2};
		msg.ride.ids = vector<uint32_t>{3, 4};
		msg.ride.s = "abc";
		msg.ride.m[1] = 2;
		msg.ride.m[11] = 22;

		msg.vecData.push_back(Data{ 1,vector<uint32_t>{2,3} });
		msg.mapData[2] = Data{ 2,vector<uint32_t>{2,3} };
		msg.mapData[3] = Data{ 3,vector<uint32_t>{22,33} };
		strMsg = PackToString(msg);
	}
	//模拟接收网络字节流，处理消息
	{
		ConClass con;
		ProtoMgr::Ins().Dispatch(con, strMsg.c_str(), strMsg.length());
	}

	{
		proto::insert_sc msg;
		msg.ret = true;
		msg.a = 6;
		strMsg = PackToString(msg);

		ConClass con;
		ProtoMgr::Ins().Dispatch(con, strMsg.c_str(), strMsg.length());
	}
	{
		proto::test2_sc msg;
		msg.ret = true;
		strMsg = PackToString(msg);

		ConClass con;
		UNIT_INFO("NEXT LINE ERROR IS OK");
		ProtoMgr::Ins().Dispatch(con, strMsg.c_str(), strMsg.length());
	}

	{//测试 动态变量，或者叫 联合体
		proto::test2_sc msg;
		msg.ret = false;
		proto::Data data{ 11,vector<uint32_t>{10,11} };
		
		msg.any = PackToString(data);

		//////////////unpack///////////
		proto::Data d2;
		size_t len= msg.any.length();
		const char *p = msg.any.c_str();
		//可以写 case 1:Unpack<xx1>; case 2:Unpack<xx2>
		proto::Unpack<proto::Data>(d2, p, len);
		UNIT_INFO("%d %d", data.id, d2.id);
		UNIT_ASSERT(data.id == d2.id);
		UNIT_ASSERT(2 == d2.ids.size());
		UNIT_ASSERT(data.ids.size() == d2.ids.size());
		UNIT_ASSERT(data.ids[0] == d2.ids[0]);
		UNIT_ASSERT(data.ids[1] == d2.ids[1]);



	}
}

namespace
{
	static void mem_insert_cs(ConClass &con, const insert_cs &msg_)
	{
		UNIT_INFO("mem_insert_cs");
	}
}

UNITTEST(testProtoMemLeak)
{
	return;

	ProtoMgr::Ins().RegMsgHandler(mem_insert_cs);
	for (;;)
	{
		string strMsg;
		{ //构建消息，发送
			insert_cs msg;
			msg.a = 1;
			msg.vecInt = vector<uint32_t>{ 1, 2 };
			msg.ride.ids = vector<uint32_t>{ 3, 4 };
			msg.ride.s = "abc";
			msg.ride.m[1] = 2;
			msg.ride.m[11] = 22;

			strMsg = PackToString(msg);
		}
		//模拟接收网络字节流，处理消息
		{
			ConClass con;
			ProtoMgr::Ins().Dispatch(con, strMsg.c_str(), strMsg.length());
		}
	}
}

