
#include "../lib_prj/CppProto/ProtoMgr.h"
#include "../lib_prj/CppProto/Proto1.h"
#include "../lib_prj/CppProto/StructPack.h"
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
		strMsg = ProtoMgr::Ins().Pack(msg);
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
		strMsg = ProtoMgr::Ins().Pack(msg);

		ConClass con;
		ProtoMgr::Ins().Dispatch(con, strMsg.c_str(), strMsg.length());
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

			strMsg = ProtoMgr::Ins().Pack(msg);
		}
		//模拟接收网络字节流，处理消息
		{
			ConClass con;
			ProtoMgr::Ins().Dispatch(con, strMsg.c_str(), strMsg.length());
		}
	}
}

