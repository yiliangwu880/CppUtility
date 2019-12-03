/*
brief:��Ϸǩ�����

*/

#pragma once
#include "../utility/typedef.h"
#include "../time/cd.h"
#include "../time/sys_time.h"
#include "../time/alarm_clock.h"
#include <vector>

/*
�ۻ�ÿ��ǩ������
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
	time_t m_last_num;	//���������
	uint32 m_cnt;		//ǩ����
};

/*
��¼ÿ������ǩ��
δ����
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
	time_t m_last_week;		//�����������
	time_t m_last_day;		//�����������
	VecBool m_vec_bool;		//ÿһ��ǩ����¼, idx==0��ʾ��1
	uint32 m_sign_cnt;
};

/*
��¼ÿ������ǩ��
δ����
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
	time_t m_last_day;		//������ʵ���������
	time_t m_last_mon;		//������ʵ���������
	VecBool m_vec_bool;		//ÿһ��ǩ����¼,0��ʾ��1
	uint32 m_sign_cnt;
};