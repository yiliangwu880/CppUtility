/*
brief:
*/
#include "cache.h"

namespace{

	CacheStateEnum s_arr[4][6]=
	{
		 //内容为行为后的新状态 或者 表示出错行为.
		/*右边旧状态	NEW,			DEL,			MOD,			CLEAR,			NONE,			ERR*/
		//下面是行为 
		/*NEW*/		{	CACHE_S_ERROR,	CACHE_S_MOD,	CACHE_S_ERROR,	CACHE_S_ERROR,	CACHE_S_ERROR,	CACHE_S_ERROR},
		/*DEL*/		{	CACHE_S_NONE,	CACHE_S_ERROR,	CACHE_S_DEL,	CACHE_S_DEL,	CACHE_S_ERROR,	CACHE_S_ERROR},
		/*MOD*/		{	CACHE_S_NEW,	CACHE_S_ERROR,	CACHE_S_MOD,	CACHE_S_MOD,	CACHE_S_ERROR,	CACHE_S_ERROR},
		/*CLEAR*/	{	CACHE_S_CLEAR,	CACHE_S_NONE,	CACHE_S_CLEAR,	CACHE_S_CLEAR,	CACHE_S_ERROR,	CACHE_S_ERROR},
	};
	CacheStateEnum changeState(CacheStateEnum old_state, CacheStateEnum action)
	{
		return s_arr[action][old_state];
	}
}


CacheChangeStateRet CacheState::changeState( CacheStateEnum action )
{
	switch(action)
	{
	default:
		return CCSR_ERROR;
		break;
	case CACHE_S_NEW:
	case CACHE_S_DEL:
	case CACHE_S_MOD:
	case CACHE_S_CLEAR:
		CacheStateEnum ret =  ::changeState(m_state, action);
		if (CACHE_S_ERROR == ret)
		{
			return CCSR_ERROR;
		}
		else if (CACHE_S_NONE == ret)
		{
			return CCSR_CHANGE_INVALID;
		}
		else
		{
			return CCSR_CHANGE;
		}
		break;
	}
	return CCSR_ERROR;
}

CacheStateEnum CacheState::state() const
{
	return m_state;
}

bool CacheState::dirty() const
{
	if(CACHE_S_NONE == m_state)
	{ //非dirty, 也非clear
		return false;
	}
	return CACHE_S_CLEAR != m_state;
}

