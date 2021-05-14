
#include "Proto1.h"
#include "StructPack.h"
#include "HandleProto.h"

namespace proto
{
	ProtoMgr::ProtoMgr()
	{
		Check();
	}

	void ProtoMgr::Dispatch(ConClass &con, const char *msg, size_t len)
	{
		using ComMsgFun = void(ConClass &con, const char &); //消息回调函数的抽象。 
		using ComUnpackFun = bool(char &, CPointChar &, size_t &);//消息解析函数的抽象。 

		uint16_t cmdId = *(const uint16_t *)msg; //约定协议前 uint16_t 为 cmdId. 
		auto it = m_id2MsgData.find(cmdId);
		if (it == m_id2MsgData.end())
		{
			L_ERROR("unknow cmdId %d", cmdId);
			return;
		}
		MsgData &msgData = it->second;
		ComMsgFun *fun = (ComMsgFun *)msgData.msgFun;
		ComUnpackFun *unpack = (ComUnpackFun *)msgData.unpackFun;
		char *msgType = msgData.createFun();
		if (unpack(*msgType, msg, len))
		{
			(*fun)(con, *msgType);
		}
		msgData.freeFun(msgType);
	}

	void ProtoMgr::Check()
	{
#pragma pack(push)
#pragma pack(1)
#if 0

		//宏展开代码模板
		struct Check_insert_cs
		{
			using CheckType = insert_cs;
			decltype(CheckType::id) id;
			decltype(CheckType::a) a;
			decltype(CheckType::vecInt) vecInt;
			decltype(CheckType::ride) ride;
		};
		L_INFO("%d %d", sizeof(Check_insert_cs), sizeof(Check_insert_cs::CheckType));
		L_ASSERT(sizeof(Check_insert_cs) == sizeof(Check_insert_cs::CheckType)); //确保域宏定义完整
			//宏实现的代码模板
		{
			Ride t;
			size_t lastOffset = 0; //用来检查定义顺序是否和结构一致
			//field 1
			{
				size_t offset = (size_t)&(((decltype(t) *)(nullptr))->id);
				L_ASSERT(lastOffset <= offset); //field定义顺序和执行不一致
				lastOffset = offset;
			}
			//field 2
			//。。。。。
		}
#endif	
	


#define DB_CLASS_NAME(className) \
	struct Check_##className \
	{\
		using CheckType = className;\

#define DB_FIELD(fieldName)	\
		decltype(CheckType::fieldName) fieldName;\

#define DB_CLASS_END\
	};\

		DB_ALL_STRUCT_INFO

#undef  DB_CLASS_NAME
#undef  DB_FIELD
#undef  DB_CLASS_END

#define DB_CLASS_NAME(className) \
	  if(sizeof(Check_##className) != sizeof(Check_##className::CheckType))\
	  {\
			L_ERROR(#className" miss field define!");\
			L_ASSERT(false);\
	  }\

#define DB_FIELD(fieldName)	
#define DB_CLASS_END

			DB_ALL_STRUCT_INFO

#undef  DB_CLASS_NAME
#undef  DB_FIELD
#undef  DB_CLASS_END



#define DB_CLASS_NAME(className) \
	{\
		className t;\
		size_t lastOffset = 0; \

#define DB_FIELD(fieldName)	\
	{\
		size_t offset = (size_t)&(((decltype(t) *)(nullptr))->fieldName);\
		L_ASSERT(lastOffset <= offset); \
		lastOffset = offset;\
	}\

#define DB_CLASS_END\
	}\

			DB_ALL_STRUCT_INFO

#undef  DB_CLASS_NAME
#undef  DB_FIELD
#undef  DB_CLASS_END

#pragma pack(pop)
	}


}

