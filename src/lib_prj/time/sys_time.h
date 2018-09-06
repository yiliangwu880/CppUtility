/*
	brief: 时间相关

*/

#pragma once
#include "../utility/typedef.h"
#include "../utility/singleton.h"
#include "cd.h"
#include <time.h>
#include <vector>
#include <map>

namespace wyl
{
    //参数描述中的术语年月： 表示实际数字，比如 2015 1 就是2015年1月
    //time_t表达：  格林威治时间1970 年 1 月 1 日 00:00:00 开始计算的秒数。 换成中国时间是 1970-01-01 08:00:00
	//使用方法：用定时器每秒调用g_time.refresh()，其他时间不刷新。好处，比每次调用刷新效率高一点， 函数运行过程中，g_time不会发生变化。避免时间变化引起的bug
	//使用例子：
	/*
		main_loop()
		{
			g_time.refresh();刷新当前系统时间
			const tm &t = g_time.getTm();
		}

	*/
	class SysTime : public SingletonBase<SysTime>
	{
        friend class SingletonBase<SysTime>;
        SysTime();

	public:
        //para1: int y,  年   
        //para2: int m,  月
		static int GetDaysInMonth(int y,int m);
        
        //返回月的周期数。
        //para1: int y_idx,  第几年，0开始。
        //para2: int m,  月
		static int GetMonthPeriodCnt(int y_idx, int m);

        //是闰年
		static bool IsLeapYear(int y);

		//获取time_t所在天的零时
		static time_t GetCurDayStart(time_t sec);
		//获取time_t所在周的第一天的零时, 失败返回0
		static time_t GetCurWeekStart(time_t sec);
		//获取time_t所在月的第一天的零时, 失败返回0
		static time_t GetCurMonStart(time_t sec);
		//获取time_t所在年的第一天的零时, 失败返回0
		static time_t GetCurYearStart(time_t sec);

		//!将用"%Y-%m-%d-%H-%M-%S"表示的时间转换成time_t
		static bool TimeStrToTime_t(const char* szTimeString, time_t& ret_time);
		//!将用"%Y-%m-%d-%H-%M-%S"表示的时间转换成tm表示的结构
		static bool TimeStringToTm(const char* szTimeString, tm& ret_tm);
		//!将用tTime表示的时间转换成用"%Y-%m-%d %H-%M-%S"表示
		static bool TimeToTimeStr(time_t t, std::string &str);
	public:
		//刷新当前日历时间
		void Refresh();
		//返回系统的当前日历时间的秒数
        inline time_t Sec() const{return m_sec;};
		//返回当前日历格式
		const tm &CurTm() const;
		bool IsLeapYear() const;
		//返回当前月有多少天
		int GetDaysInMonth() const;
		//返回月的周期数，从1970..开始计算. 1开始
		int GetMonthPeriodCnt() const;
		//返回天的周期数，从1970-01-04开始计算
		int GetDayPeriodCnt() const;
		//返回星期的周期数，从1970-01-04开始计算
		int GetWeekPeriodCnt() const;
        std::string GetTimeStr() const;

        //调试用
        /////////////////////////////////////////
        //设置系统时间的偏移。 
        void SetTimeOffset(time_t offset);
        //加秒偏移
        void AddTimerOffset(time_t offset);
        //将格式"%Y-%m-%d-%H-%M-%S"表示的时间 设置为当前时间
        //数字必须为4位或者2位
        bool SetTimeByStr(const char *pTime);
        //加月偏移
        void AddMonthOffset(unsigned int month);
        //加年偏移
        void AddYearOffset(unsigned int year);
        /////////////////////////////////////////

	private:
		time_t m_sec;
		tm m_tm;
		CurCycleNum m_day_period_cnt;
        CurCycleNum m_week_period_cnt;
        time_t m_offset;
	};
	extern SysTime &g_time;
	

	// 计时器，操作：开始，暂停，恢复，获取流逝时间
	class Timer
    {
    public:
        enum State{
            TIMER_S_NULL,
            TIMER_S_RUN,
            TIMER_S_PAUSE,
        };
	public:
		Timer()
			:m_start(0)
			,m_pause(0)
			,m_state(TIMER_S_NULL)
		{
		}
        State state();
		void start();
		void pause();
		void resume();
		//返回运行的时间但不结束
		time_t peerCurrentTimer() const;
		//返回运行的时间
		time_t end();
	private:
		time_t m_start;
		time_t m_pause;
        State m_state;
	};
}//namespace wyl