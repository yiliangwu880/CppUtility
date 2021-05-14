
#include "Proto1.h"
#include "StructPack.h"
#include "ProtoMgr.h"

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

		uint16_t msgId = *(const uint16_t *)msg; //约定协议前 uint16_t 为 cmdId. 
		auto it = m_id2MsgData.find(msgId);
		if (it == m_id2MsgData.end())
		{
			L_ERROR("unknow cmdId %d", msgId);
			return;
		}
		MsgData &msgData = it->second;
		ComMsgFun *fun = (ComMsgFun *)msgData.msgFun;
		ComUnpackFun *unpack = (ComUnpackFun *)msgData.unpackFun;
		char *msgType = msgData.createFun();
		if (unpack(*msgType, msg, len))
		{
			if (0 == len)
			{
				(*fun)(con, *msgType);
			}
			else//还有内容未解包
			{
				L_ERROR("have unknow bytes unpack %d", len);
			}
		}
		else
		{
			L_ERROR("unpack fail, cmdId=%d", msgId);
		}
		msgData.freeFun(msgType);
	}

	void ProtoMgr::Check()
	{
#pragma pack(push)
#pragma pack(1)

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
		if(lastOffset > offset)\
		{\
			L_ERROR("field "#fieldName" define order is error");\
			L_ASSERT(false);\
		}\
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

