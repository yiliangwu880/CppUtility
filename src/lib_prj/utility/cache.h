/*
brief:缓存内存
一般简单的不需要用这种，直接bool类型表示是否dirty就可以，new, del操作要实时。
*/
#pragma once
#include "typedef.h"


enum CacheStateEnum
{
	CACHE_S_NEW,
	CACHE_S_DEL,
	CACHE_S_MOD,
	CACHE_S_CLEAR, //未修改的，和存档一致
	CACHE_S_NONE, //不存在

	//以下为非缓存状态
	CACHE_S_ERROR,  //表示错误状态

};

//缓存改变动作结果
enum CacheChangeStateRet
{
	CCSR_CHANGE,			//改变
	CCSR_CHANGE_INVALID,	//改变并且缓存无效
	CCSR_ERROR,
};
class CacheState
{
public:
	//新对象有两种状态，新建缓存或者读档缓存
	explicit CacheState(bool isNew = false)
	{
		if (isNew)
		{
			m_state = CACHE_S_NEW;
		}
		else
		{
			m_state = CACHE_S_CLEAR;
		}
	}
	//注意：return CCSR_CHANGE_INVALID时应该删除缓存。
	CacheChangeStateRet changeState(CacheStateEnum action);
	CacheStateEnum state() const;
	bool dirty() const;
	
private:
	CacheStateEnum m_state;
};

void dkdjdk()
{
	CacheState a;
	//a.m_state = CacheState::DEL; 
}