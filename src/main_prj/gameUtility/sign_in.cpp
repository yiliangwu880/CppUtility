

#include "sign_in.h"
#include "../time/sys_time.h"
#include "../time/alarm_clock.h"



bool AccumulSignIn::signIn()
{
	time_t cur = m_cycle_num.GetCurrent();
	if (cur != m_last_num)
	{
		m_last_num = cur;
		++m_cnt;
		return true;
	}
	return false;
}

void WeekSignIn::Reset()
{
	m_vec_bool.clear();
	m_vec_bool.resize(7);
	m_sign_cnt=0;
}

bool WeekSignIn::signIn()
{
	using namespace wyl;

	time_t day = m_day_cnt.GetCurrent();
	if (m_last_day==day)
	{
		return false;
	}
	m_last_day = day;

	time_t week = m_week_cnt.GetCurrent();
	if (m_last_week != week)
	{
		Reset();
		m_last_week = week;
	}

	const tm &tm_d = g_time.CurTm();
	m_vec_bool[tm_d.tm_wday] = true;
	++m_sign_cnt;
	return true;
}

void MonthSignIn::Reset()
{	
	wyl::g_time.Refresh();
	m_vec_bool.clear();
	m_vec_bool.resize(wyl::g_time.GetDaysInMonth());
	m_sign_cnt=0;
}

bool MonthSignIn::signIn()
{
	using namespace wyl;
	time_t cur = m_day_cnt.GetCurrent();
	if (m_last_day==cur)
	{
		return false;
	}
	m_last_day = cur;

	int cur_mon = g_time.GetMonthPeriodCnt();
	if (m_last_mon != cur_mon)
	{
		m_last_mon = cur_mon;
		Reset();
	}

	const tm &tm_d = g_time.CurTm();
	m_vec_bool[tm_d.tm_mday-1]= true;
	++m_sign_cnt;
	return true;
}
