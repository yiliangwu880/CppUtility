/*
brief:处理延时处理逻辑， 比如:处理离线玩家事件
例子：
	class MyTarget: public BaseDelayTarget
	class MyOpt: public BaseDelayOpt
	{
		virtual void Handle(BaseDelayTarget &target){....};
	};
	class MyOptMgr: public BaseDelayOptMgr
	{
	public:
		virtual BaseDelayTarget *FindTarget(uint64 target_id)
		{
			//实现查找内存目标
		};			
		virtual void MissTarget(uint64 target_id)
		{
			//实现，请求读档。  (缓存没有对象时会调用）
		};				
	};

	MyOptMgr mgr;
	//读档成功或者找到target调用
	mgr.OptTarget(id, target);
	//读档失败调用
	mgr.DelOpt(id);

	//加入操作请求
	MyOptMgr mgr;
	MyOpt *opt =  mgr.CreateOpt<MyOpt>();
	//todo 用户自定义opt初始化代码
	mgr.AddOpt(target_id, opt);
*/

#pragma once
#include "../utility/typedef.h"
#include <vector>
#include <map>
#include <utility>

//操作目标
class BaseDelayTarget
{
public:
	virtual ~BaseDelayTarget(){};
};

//缓存操作
class BaseDelayOpt
{
public:
	virtual ~BaseDelayOpt(){};
	virtual void Handle(BaseDelayTarget &target) = 0;
};

class BaseDelayOptMgr
{
public:
	BaseDelayOptMgr();


	//请求创建BaseDelayOpt派生对象
	//特点：错误调用也不会内存泄露
	//注意：创建后，马上调用AddOpt后，才能调用BaseDelayOptMgr的其他成员。不然会下一次CreateOpt()会销毁
	template <typename DeriveOpt>
	DeriveOpt *CreateOpt()
	{
		if (NULL != m_new_opt)
		{
			delete m_new_opt;
			printf("error call, call continue CreateOpt twice!!!\n");
			m_new_opt = NULL;
		}
		DeriveOpt *t = new DeriveOpt();
		if(NULL == t)
		{
			return NULL;
		}
		m_new_opt = t;
		return t;
	}
	//c++11用这个版本代替上面的函数
	template <typename DeriveOpt, typename... Args>
	DeriveOpt *CreateOpt(Args&&... args)
	{
		if (NULL != m_new_opt)
		{
			delete m_new_opt;
			printf("error call, call continue CreateOpt twice!!!\n");
			m_new_opt = NULL;
		}
		DeriveOpt *t = new DeriveOpt(std::forward<Args>(args)...);
		if (NULL == t)
		{
			return NULL;
		}
		m_new_opt = t;
		return t;
	}

	void AddOpt( uint64 target_id, BaseDelayOpt *opt );			//加一个操作.		(目标找到马上执行，不在就等调用  HandleTarget 再操作)
	void OptTarget(uint64 target_id, BaseDelayTarget &target);	//对目标操作缓存操作
	void DelOpt(uint64 target_id);								//调用删除目标缓存操作  (读档失败，目标不存时调用)

	//for test use
	int GetOptNum(uint64 target_id);
private:
    virtual BaseDelayTarget *FindTarget(uint64 target_id)=0;			//实现查找内存目标
	virtual void MissTarget(uint64 target_id) = 0;				//实现，请求读档。  (缓存没有对象时会调用）

public:
	static const uint32 MAX_REQ_NUM_PER_TARGET = 3;  //每个target最大缓存请求数

private:
	typedef std::vector<BaseDelayOpt *> VecBaseDelayOpt;
    typedef std::map<uint64, VecBaseDelayOpt> Id2VecOpt;


	Id2VecOpt m_id_2_vec_opt;
	BaseDelayOpt *m_new_opt;		//new出来opt对象，加入延时操作时，对象转移到m_vec_opt
	bool m_is_opting;				//true表示进入BaseDelayOptMgr::OptTarget运行中
};

