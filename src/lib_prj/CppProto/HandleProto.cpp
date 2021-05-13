
#include "Proto1.h"
#include "StructPack.h"
#if 0

//网络链接对象,实际项目自己定义
class ConClass
{

};

struct MsgData
{
	void *unpackFun = nullptr;	//解包函数  bool Unpack(T &t, CPointChar &cur, size_t &len)
	void *msgFun = nullptr;		//处理消息函数	 void(*fun)(ConClass &con, const MsgType &msg)
	char *(*createFun)() = nullptr;		//消息对象 分配函数  
	void (*freeFun)(char *) = nullptr;	//消息对象 释放函数 
};

class ProtoMgr : public Singleton<ProtoMgr>
{
	std::unordered_map<uint16_t, MsgData> m_id2MsgData; //proto 消息ID 2 回调
	
public:
	//注册 proto消息回调
	template<class MsgType>
	void RegProtoParse(void(*fun)(ConClass &con, const MsgType &msg))
	{
		MsgType msg;
		if (m_id2MsgData.find(msg.id) != m_id2MsgData.end())
		{
			L_ERROR("repeated reg");
			return;
		}
		MsgData &d = m_id2MsgData[msg.id];
		d.msgFun = (void *)fun;
		d.unpackFun = (void *)proto::Unpack<MsgType>;
		d.createFun = (void *)proto::CreateFun<MsgType>;
		d.freeFun = (void *)proto::FreeFun<MsgType>;
	}

	void OnRecv(ConClass &con, const char *msg, size_t len)
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
		ComUnpackFun *unpack = (ComMsgFun *)msgData.unpackFun;
		char *msgType = msgData.createFun();
		unpack(*msgType, msg, len);
		(*fun)(con, *msgType);
		msgData.freeFun(msgType);
	}
};

#endif

namespace proto
{
#if 0
	ProtoMgr::ProtoMgr()
	{
		Check();
	}

	void ProtoMgr::Dispach(ConClass &con, const char *msg, size_t len)
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
		ComUnpackFun *unpack = (ComMsgFun *)msgData.unpackFun;
		char *msgType = msgData.createFun();
		unpack(*msgType, msg, len);
		(*fun)(con, *msgType);
		msgData.freeFun(msgType);
	}
#endif
#if 0

	void ProtoMgr::Check()
	{
#if 0
		//宏展开代码模板
		struct Check_Ride
		{
			using CheckType = Ride;
			decltype(CheckType::id) id;
			decltype(CheckType::ids) ids;
		};
		L_ASSERT(sizeof(Check_Ride) == sizeof(Check_Ride::CheckType)); //确保域宏定义完整

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
	L_ASSERT(sizeof(Check_##className) == sizeof(Check_##className::CheckType));\

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

	}


#endif
}

