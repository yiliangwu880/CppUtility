/*
brief:游戏签到相关

*/

#pragma once
#include "../utility/typedef.h"
#include "../time/cd.h"
#include "../time/sys_time.h"
#include "../time/alarm_clock.h"
#include <vector>

/*
累积每日签到数，
*/
class AccumulSignIn
{
public:
	AccumulSignIn()
		:m_cycle_num(3600*24,0)
		,m_last_num(0)
		,m_cnt(0)
	{}
	bool signIn();
	inline uint32 getSignCnt() const{return m_cnt;}

private:
	CurCycleNum m_cycle_num;
	time_t m_last_num;	//最近周期数
	uint32 m_cnt;		//签到数
};

/*
记录每周那天签到
未测试
*/
class WeekSignIn
{
public:
	typedef std::vector<bool> VecBool;
public:
	WeekSignIn()
		:m_week_cnt(3600*24*7)
		,m_day_cnt(3600*24)
		,m_last_week(0)
		,m_last_day(0)
		,m_sign_cnt(0)
	{}
	bool signIn();
	void Reset();
	const VecBool &getSignVecBool() const{return m_vec_bool;}
	const uint32 getSignCnt() const {return m_sign_cnt;}

private:
	CurCycleNum m_week_cnt;
	CurCycleNum m_day_cnt;
	time_t m_last_week;		//最近周周期数
	time_t m_last_day;		//最近日周期数
	VecBool m_vec_bool;		//每一天签到记录, idx==0表示周1
	uint32 m_sign_cnt;
};

/*
记录每月那天签到
未测试
*/
class MonthSignIn
{
public:
	typedef std::vector<bool> VecBool;
public:
	MonthSignIn()
		:m_day_cnt(3600*24)
		,m_last_day(0)
		,m_last_mon(0)
		,m_sign_cnt(0)
	{}
	bool signIn();
	void Reset();
	const VecBool &getSignVecBool() const{return m_vec_bool;}
	const uint32 getSignCnt() const {return m_sign_cnt;}

private:
	CurCycleNum m_day_cnt;
	time_t m_last_day;		//最近访问的天周期数
	time_t m_last_mon;		//最近访问的月周期数
	VecBool m_vec_bool;		//每一天签到记录,0表示周1
	uint32 m_sign_cnt;
};