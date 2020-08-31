/*
参考用吧：一般代码设计成简单点。 不需要用这种，不然有点复杂，可阅读性低。
设计简单点比如：player 发送消息前，参数保存在player对象里面，等回调的时候再通过playerid去取完事。

brief: 延时参数管理， 用户动态调用创建初始化， 延时（收到网络消息后），获取参数，自动释放。
例子：
	网络请求例子
	//网络请求是，构造动态参数
	class MyPara : public BaseDelayPara
	{
	public:
	MyPara(int p1, int p2)
	{
	a = p1; b = p2;
	}
	int a, b;
	};

	MyPara *p = DelayParaMgr::Instance().ConstructPara<MyPara>(1, 2);
	assert(p != NULL);
	send(p); --发送到网络，等回调


	//网络回调时调用
	DelayParaGuard<MyPara> guard(p); //自动释放 p内存
	assert(guard.m_para != NULL);
	assert(guard.m_para->a == 1);


	 //超时清理参数
	 DelayParaMgr::Instance().OnTimer();
	
*/

#pragma once
#include "../../svr_util/include/typedef.h"
#include <vector>
#include <map>
#include <utility>



class BaseDelayPara
{
public:
	virtual ~BaseDelayPara() = 0;

private:

};

//template <typename T> class DelayParaGuard; //pal朋友

class DelayParaMgr
{
	template <typename T> 
	friend class DelayParaGuard; //DelayParaGuard类的所有实例化, 都为DelayParaMgr的友元

	DelayParaMgr();
public:
	static DelayParaMgr &Instance()
	{
		static DelayParaMgr obj;
		return obj;
	}

	//请求创建
	//template <typename DelayPara>
	//DelayPara *ConstructPara()
	//{
	//	DelayPara *para = new DelayPara();
	//	if(NULL == para)
	//	{
	//		return NULL;
	//	}
	//	time_t cur=0;
	//	time(&cur);
	//	m_para_2_time.insert(std::make_pair(para, cur));
	//	return para;
	//}
	//c++11用这个版本代替上面的函数
	template <typename DelayPara, typename... Args>
	DelayPara *ConstructPara(Args&&... args)
	{
		DelayPara *para = new DelayPara(std::forward<Args>(args)...);
		if (NULL == para)
		{
			return NULL;
		}
		time_t cur = 0;
		time(&cur);
		m_para_2_time.insert(std::make_pair(para, cur));
		return para;
	}
	//读取参数，不释放。
	template <typename DelayPara>
	const DelayPara *PeerPara(BaseDelayPara *para)
	{
		BaseDelayPara *base = DelayParaMgr::Instance().FindPara(para);
		return dynamic_cast<DelayPara *>(base);
	}

	//建议5秒调用一次
	void OnTimer();
	uint32_t GetParaCnt();
	//一般不设置，用默认的就行
	void SetTimeOutSec(uint64 sec);

private:
	void DeletePara(BaseDelayPara *para);
	//返回参数指针， 失败返回null
	BaseDelayPara *FindPara(BaseDelayPara *para);

private:
	static const uint64 TIME_OUT_SEC = 60*5;  //过期时间，秒. 时间可以长点，避免动态内存重复利用产生BUG。 回调参数获取的内存不是自己的。
	typedef std::map<BaseDelayPara *, uint64> Para2Time;

	Para2Time m_para_2_time; //参数 2 创建时间戳
	uint64 m_time_out_sec = 10;
};



template <typename DelayPara>
class DelayParaGuard
{
public:
	DelayParaGuard(BaseDelayPara *para)
	:m_para(NULL)
	{
		BaseDelayPara *base = DelayParaMgr::Instance().FindPara(para);
		m_para = dynamic_cast<DelayPara *>(base);
	}
	~DelayParaGuard()
	{
		if (NULL != m_para)
		{
			DelayParaMgr::Instance().DeletePara(m_para);
		}
	}

	DelayPara *m_para;
};
