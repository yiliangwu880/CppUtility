/*
author:yiliangwu880
	you can get more refer from https ://github.com/yiliangwu880/CppUtility.git

C++结构体作为协议，提供给C++进程间通讯。
源码的方式提供复用，需要根据项目小量修改

*/


#include <string>
#include <vector>
#include <unordered_map>
#include "ProtoInclude.h"

namespace proto
{
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

	class ProtoMgr 
	{
		std::unordered_map<uint16_t, MsgData> m_id2MsgData; //proto 消息ID 2 回调

	public:
		static ProtoMgr& Ins()
		{
			static ProtoMgr ins;
			return ins;
		}

		ProtoMgr();
		//注册 proto消息回调
		template<class MsgType>
		void RegMsgHandler(void(*fun)(ConClass &con, const MsgType &msg));

		void Dispatch(ConClass &con, const char *msg, size_t len);

	private:
		void Check();//检查宏定义是否正确。
	};

	template<class MsgType>
	void ProtoMgr::RegMsgHandler(void(*fun)(ConClass &con, const MsgType &msg))
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
		d.createFun = (decltype(d.createFun))proto::CreateFun<MsgType>;
		d.freeFun = (decltype(d.freeFun))proto::FreeFun<MsgType>;
	}

}