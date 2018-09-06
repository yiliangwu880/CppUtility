/*
author: YiliangWu
实现cd相关
)普通cd。 
)累积倒计时到指定数值，进入强制冷却状态
)周期特性，比如周一冷却cd。
)时间周期性增长的数值，比如游戏中每分钟加1体力。

*/


#include <time.h>
#include "cd.h"
#include "sys_time.h"
#include "../utility/misc.h"
#include <stdio.h>
#include <algorithm>

using namespace wyl;


CurCycleNum::CurCycleNum( const time_t &period, const time_t &limit/* = 0*/ )
:m_period(period)
,m_start(START_SEC+limit)
{
	if(limit>=period)
    {
        printf("input para error, %ld", limit);
        m_start = START_SEC;
    }
}

time_t CurCycleNum::Get( const time_t &cur_sec ) const
{	
	if (cur_sec<START_SEC)
	{
		return 0;
	}

	time_t dist_sec=cur_sec-m_start;
	time_t period_num = dist_sec/m_period;
	return period_num;
}

time_t CurCycleNum::GetCurrent() const
{
	return Get(g_time.Sec());
}



time_t CurCycleNum::GetCurPeriodNum( time_t start, time_t period )
{
	time_t cur = g_time.Sec();
	if (start>cur)
	{
		return 0;
	}

	time_t dist = cur - start;
	return dist/period;
}

PeriodCnt::PeriodCnt( const time_t &period, const time_t &limit )
:m_last(time_t())
,m_current_period_num(period, limit)
{    
	m_last = m_current_period_num.GetCurrent();
}

time_t PeriodCnt::getCnt()
{
	time_t current = m_current_period_num.GetCurrent();
	time_t temp = m_last;
	m_last = current;
	return current-temp;
}

time_t PeriodCnt::previewCnt() const
{
	time_t current = m_current_period_num.GetCurrent();
	return current-m_last;
}





CdDefault::CdDefault( const time_t& period, time_t start )
:m_start(start)
,m_period(period)
{
}

bool CdDefault::IsCD() const
{
	const time_t current = g_time.Sec();
	if (current<m_start)
	{
		return true;
	}
	if(current-m_start < m_period)
	{
		return true;
	}
	return false;
}

time_t CdDefault::GetCountDownTime() const
{
	const time_t current = g_time.Sec();
	if (current<m_start)
	{
		return m_period+m_start-current;
	}
	if(current-m_start < m_period)
	{
		return m_period-(current-m_start);
	}
	return time_t();
}

void CdDefault::ChangePeriod( const time_t& period )
{
	if (!IsCD())
	{
		m_start=time_t();
	}
	m_period = period;
}



void CdDefault::StartCD()
{
	m_start=g_time.Sec();
}

void CdDefault::ClearCD()
{
	m_start=time_t();
}

PeriodAccumulate::PeriodAccumulate( const time_t &period, uint32 max_num )
:m_num(max_num)
,m_max_num(max_num)
,m_last_cycle_cnt(0)
,m_period_cnt(period)
{
}

uint32 PeriodAccumulate::currentNum()
{
	time_t cur = m_period_cnt.GetCurrent();
	time_t temp = m_last_cycle_cnt;
	m_last_cycle_cnt = cur;
	uint32 add = 0;
	if (cur>temp)
	{
		add =(uint32)(cur-temp);
	}

	if (m_num>=m_max_num)
	{
		return m_num;
	}
	m_num += add;
	if (m_num>m_max_num)
	{
		m_num = m_max_num;
	}
	return m_num;
}

void PeriodAccumulate::costNum( uint32 cost )
{
	currentNum();
	if (cost>m_num)
	{
		m_num = 0;
		return;
	}
	m_num-=cost;
}

void PeriodAccumulate::addExtNum( uint32 add )
{
	currentNum();
	m_num+=add;
}

void PeriodAccumulate::setMaxNum( uint32 max_num )
{
	m_max_num = max_num;
}


CycleMoreReset::CycleMoreReset( const time_t &period, const VecInt64 &vec_limit )
	:m_period(period)
{
	m_vec_limit = vec_limit;
	std::sort(m_vec_limit.begin(), m_vec_limit.end());

	for (VecInt64::const_iterator it=m_vec_limit.begin(); it!=m_vec_limit.end(); ++it)
	{
		if(*it>=period)
		{
			printf("input para error, %lld", *it);
		}
	}	
}

time_t CycleMoreReset::GetCurCycleNum() const
{
	time_t cur_sec = g_time.Sec();
	if (cur_sec<CurCycleNum::START_SEC)
	{
		return 0;
	}

	time_t dist_sec=cur_sec-CurCycleNum::START_SEC;
	time_t major_period = dist_sec/m_period;  
	time_t remainder = dist_sec%m_period;  

	time_t add = 0;
	for (VecInt64::const_iterator it=m_vec_limit.begin(); it!=m_vec_limit.end(); ++it)
	{
		if(remainder>=*it)
		{
			++add;
		}
	}	

	return major_period*m_vec_limit.size()+add;
}
