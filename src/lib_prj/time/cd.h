/*
author: YiliangWu
实现cd相关
*/

#pragma once

#include <assert.h>
#include <time.h>
#include "../utility/typedef.h"
#include "../utility/cnt_typedef.h"


/*
brief: 倒计时功能
criteria:
) 初始化正常状态,调用startCD进入cd状态
) 时间流逝过CD周期后，会进入正常状态
*/
class CdDefault
{
public:
	//param: start	cd开始时间
	explicit CdDefault(const time_t& period, time_t start=0);
	bool IsCD() const;
	void StartCD();
	void ClearCD();
	time_t GetCountDownTime() const;
	//改变CD周期，当前一CD冷却后，没调用isCD情况下调用，会先清CD。
	void ChangePeriod(const time_t& period);

	time_t GetStart() const {return m_start;}
	time_t GetPeriod() const {return m_period;}

private:
	time_t m_start;		
	time_t m_period;	
};

/*
brief:累积倒计时到指定数值，进入强制冷却状态
规则:
)初始化为正常状态，每次调用accumulate 都会累积倒计时间。
)倒计时间大于最大累积时间时，变成强制冷却状态
)强制冷却状态时，当倒计时间0才变成正常状态。
*/
template<typename Tick>
class AccumulCDBase
{
public:
	AccumulCDBase(const Tick &forceCdDist)
		:m_endCdTick(Tick())
		,m_isForceCd(false)
		,m_forceCdDist(forceCdDist)
	{
	}
	virtual ~AccumulCDBase(){};

	bool IsForceCD() 
	{	
		if (CurrentTick()>=m_endCdTick)
		{
			m_isForceCd = false;
		}
		return m_isForceCd;
	}

	Tick AddCD(Tick accumTick)
	{
		const Tick current = CurrentTick();
		if (current>=m_endCdTick)
		{
			m_endCdTick = current+accumTick;
		}
		else
		{
			m_endCdTick += accumTick;
		}
		Tick accum=m_endCdTick-current;
		if (accum>=m_forceCdDist)
		{
			m_isForceCd = true;
		}
		return accum;
	}

	inline void ClearCD()
	{
		m_endCdTick=Tick();
		m_isForceCd = false;
	}
	
	inline Tick GetCD()
	{
		const Tick current = CurrentTick();
		if (current>=m_endCdTick)
		{
			return 0;
		}			
		return m_endCdTick-current;
	}
	virtual Tick CurrentTick() const =0;

protected://don't access member data unless serialize control
	Tick m_endCdTick;			// cd ending tick
	bool m_isForceCd;
private:		
	Tick m_forceCdDist;			// force cd tick distance
};

/*
brief:计算第几个周期，合适周期长度为固定的类型。比如周，天，小时，不合适周期为年，月。
规则: 
)period must be one of (week,day,hours,minue,half hours,half day)
)limit<period 周期开始计算的时间（分界点）
)从北京时间1970-01-04 00:00:00，周日 为周期值0开始计算,以前的日期一律用周期值0表示

例如： 一个星期为周期， 24*3600*2（周二0时） 为分界点，某x周的星期一表示第y个周期。 那么x周的星期二表示y+1个周期。
使用例子：
	CurCycleNum obj(3600*24, 3600);  //周期为日，分界点为1点
	uint32 last_num = 0;
	while(1)
	{
		uint32 cur_num = obj.GetCurrent();  返回1970-xxx到现在的周期数
		if (last_num != cur_num)
		{
			last_num = cur_num
			//to do 每日重置
		}
	}
	
*/
class CurCycleNum
{
public:
	//para period :周期
    //para limit：周期开始计算的时间. （等于分界点时间为周期x, 等于分界点时间-1秒周期为x-1）
	explicit CurCycleNum(const time_t &period, const time_t &limit = 0);
    //返回指定时间戳的周期数 
	//parameter: const time_t &CurrentTick, 时间戳
	time_t Get(const time_t &cur_time) const;
	//返回当前时间的周期数
	time_t GetCurrent() const;

	//计算某开始时间到目前流逝周期数
	static time_t GetCurPeriodNum(time_t start, time_t period);

public:
	static const time_t START_SEC = 3*24*3600-8*3600;	//从北京时间1970-01-04 00:00:00，周日 为周期值0开始计算,以前的日期一律用周期值0表示
	// static const time_t START_SEC = 3*24*3600;   //用世界时做日历，就用这个

private:
    time_t m_period;	//周期时间长度。
	time_t m_start;		//第一个周期开始时间。	
};


//brief:
//类似CurCycleNum， 周期内，多个重置时间点。
//规则：合适周期长度为固定的类型。比如周，天，小时，不合适周期为年，月。
//
//比如：每周一，周二 FB 重置。 
//  VecInt64 vec_limit;
//	vec_limit.push_back(3600 * 24 * 1);//周一
//	vec_limit.push_back(3600 * 24 * 2);//周二
//	CycleMoreReset obj(3600 * 24 * 7, vec_limit);
//0周 周1后 周期数为1
//0周 周2后 周期数为2
//1周 周1后 周期数为3
//1周 周2后 周期数为4
//。。。

class CycleMoreReset
{
public:
	//para period 周期
	//para vec_limit 周期内再分割的时间点。 偏移值
	explicit CycleMoreReset(const time_t &period, const VecInt64 &vec_limit);
	//返回当前时间的周期数
	time_t GetCurCycleNum() const;

private:
	VecInt64 m_vec_limit;
	time_t m_period;	//周期时间长度。
};

/*
brief:流逝周期计算，可以获取历史获取到目前流逝的周期数。 (没什么复用价值，直接用过 CurCycleNum 好理解)
规则： 
	对象创建，记录当前周期数为最近历史周期数。
	调用getCount(),获取从历史周期数开始到目前流逝的周期数
 */ 
class PeriodCnt
{
public:
	PeriodCnt(const time_t &period, const time_t &limit);
	//获取从上一次调用该函数到本次调用流逝的周期数
	time_t getCnt();

	//预览最近流逝的周期数
	time_t previewCnt() const;

	time_t Save(){return m_last;}
	void Read(time_t last){m_last = last;}
private:
	time_t m_last; //最近获取的流逝周期
	CurCycleNum m_current_period_num;
};


/*
brief:用来计算 时间周期性增长的数值,比如游戏中每分钟加1体力。buf,debuf
rule:
	1 Period 为周期数，每周期数值加1， m_num 加1
	2 m_num 周期增加最大只能到m_maxNum。
	3 函数addExtNum 可以额外增加m_num，没上限。
*/

struct PeriodAccumulate
{
public:
    //para: const time_t &period, 周期
    //para: uint32 max_num, 周期回复数值最大值
	PeriodAccumulate(const time_t &period, uint32 max_num);

	uint32 currentNum();

	void costNum(uint32 cost);

	void addExtNum(uint32 add);

	void setMaxNum(uint32 max_num);

private:
	uint32 m_num;
	uint32 m_max_num;
	time_t m_last_cycle_cnt;  //最近访问周期数
	CurCycleNum m_period_cnt;	
};
