/*
时间相关
*/

#include "sys_time.h"
#include <assert.h>
#include <string>
#include "../utility/misc.h"
#include <utility>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
namespace wyl
{
	SysTime &g_time=SysTime::obj();
    using namespace std;

	void SysTime::Refresh()
	{    
		time(&m_sec); 
        m_sec+=m_offset;
		localtime_r( &m_sec, &m_tm);  
	}

	SysTime::SysTime()
		:m_sec(time_t())
		,m_day_period_cnt(24*3600)
		,m_week_period_cnt(24*3600*7)
        ,m_offset(0)
	{
		m_tm.tm_sec      =0;
		m_tm.tm_min      =0;
		m_tm.tm_hour	 =0;
		m_tm.tm_mday	 =0;
		m_tm.tm_mon	     =0;
		m_tm.tm_year	 =0;
		m_tm.tm_wday	 =0;
		m_tm.tm_yday	 =0;
		m_tm.tm_isdst	 =0;
		Refresh();
	}

	const tm & SysTime::CurTm() const
	{
		return m_tm;
	}

	int SysTime::GetDaysInMonth( int y,int m )
	{
		int day[]= {31,28,31,30,31,30,31,31,30,31,30,31};
		if (2==m)
		{
			return (IsLeapYear(y)?29:28);
		}
		else
		{
			return day[m-1];
		}
	}

	int SysTime::GetDaysInMonth() const
	{
		return GetDaysInMonth(m_tm.tm_year+1900, m_tm.tm_mon+1);
	}

	int SysTime::GetMonthPeriodCnt( int y_idx, int m )
	{
		return y_idx*12+m;
	}

	int SysTime::GetMonthPeriodCnt() const
	{
		return GetMonthPeriodCnt(m_tm.tm_year-70, m_tm.tm_mon+1);//tm_year 最小1970
	}

	int SysTime::GetDayPeriodCnt() const
	{
		return (int)m_day_period_cnt.GetCurrent();
	}

	int SysTime::GetWeekPeriodCnt() const
	{
		return (int)m_week_period_cnt.GetCurrent();
	}

	bool SysTime::IsLeapYear( int y )
	{
		return (((y) % 4 == 0 && (y) % 100 != 0) || (y) % 400 == 0);
	}

	bool SysTime::IsLeapYear() const
	{
		return IsLeapYear(1900+m_tm.tm_year);
	}

    void SysTime::SetTimeOffset( time_t offset )
    {
        m_offset = offset;
        Refresh();
    }
    void SysTime::AddTimerOffset( time_t offset )
    {
        m_offset += offset;
        Refresh();
    }

    bool SysTime::TimeStrToTime_t( const char* pTime, time_t& ret_time )
    {
        if(pTime == NULL)
        {
            return false;
        }

        tm tm_time;
        bool iRet = TimeStringToTm(pTime, tm_time);
        if(!iRet)
        {
            return false;
        }
        //strptime(pTime, "%Y-%m-%d %H-%M-%S", &tm_time); //格式化输出 win32没有这个函数,就用上面自己写的吧

        ret_time = mktime(&tm_time);
		if (ret_time  < 0)
        {
            return false;
        }
        return true;
    }

    bool SysTime::TimeStringToTm( const char* szTimeString, tm& ret_tm )
    {
        char buf[50];
        if(szTimeString == NULL || strlen(szTimeString) >= sizeof(buf))
        {
            return false;
        }

        memset(&ret_tm, 0, sizeof(ret_tm));
        memcpy(buf, szTimeString, sizeof(buf));

        uint32 str_len = strlen(buf);
        if (str_len>=sizeof(buf))
        {
            return false;
        }
        const char *pYear= &buf[0];
        const char *pMon= NULL;
        const char *pDay= NULL;
        const char *pH= "0";
        const char *pM= "0";
        const char *pS= "0";
        uint32 find_cnt = 0;
        for(uint32 i=0; i<str_len-1; ++i)   
        {     
            if (buf[i]=='-' || buf[i]==' ')
            {
                buf[i]='\0';
                find_cnt++;
                switch (find_cnt)
                {
                case 1:
                    pMon = &buf[i+1];
                    break;
                case 2:
                    pDay = &buf[i+1];
                    break;
                case 3:
                    pH = &buf[i+1];
                    break;
                case 4:
                    pM = &buf[i+1];
                    break;
                case 5:
                    pS = &buf[i+1];
                    break;
                default:
                    break;
                }
            }
        }   
		if (NULL == pMon || NULL == pDay)
		{
			return false;
		}
        //取出年
        ret_tm.tm_year = atoi(pYear) - 1900;
        if (ret_tm.tm_year<0)
        {
            return false;
        }
        ret_tm.tm_mon = atoi(pMon) -1;
        if (ret_tm.tm_mon<0 || ret_tm.tm_mon>11)
        {
            return false;
        }
        ret_tm.tm_mday = atoi(pDay);
        if (ret_tm.tm_mday<=0){return false;}

        ret_tm.tm_hour = atoi(pH);
        ret_tm.tm_min = atoi(pM);
        ret_tm.tm_sec = atoi(pS);
        return true;
    }

    bool SysTime::SetTimeByStr( const char *pTime )
    {
		if (NULL == pTime)
		{
			return false;
		}
        time_t ret_time;
        if(!TimeStrToTime_t(pTime, ret_time))
        {
            return false;
        }

        time_t t;
        time(&t); 

		time_t offset = ret_time-t;
        SetTimeOffset(offset);
        return true;
    }

    bool SysTime::TimeToTimeStr( time_t t, string &str )
    {
        tm tm_t;
        localtime_r(&t, &tm_t);

        char buf[50];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d-%02d-%02d",
            tm_t.tm_year + 1900, tm_t.tm_mon + 1, tm_t.tm_mday,
            tm_t.tm_hour, tm_t.tm_min, tm_t.tm_sec);
        assert(strlen(buf)<sizeof(buf));
        str = buf;
        return true;
    }

    std::string SysTime::GetTimeStr() const
    {
        string str;
        TimeToTimeStr(m_sec, str);
        return str;
    }

    void SysTime::AddMonthOffset( unsigned int month )
    {
        Refresh();
        tm tm_t;
        localtime_r(&m_sec, &tm_t);
        tm_t.tm_mon += month;

		if (tm_t.tm_mon>11)
		{
			tm_t.tm_year+=tm_t.tm_mon/12;
			tm_t.tm_mon = tm_t.tm_mon%12;
		}

		//调整月的日期超当前月最大值
		int max_day = GetDaysInMonth(tm_t.tm_year+1900, tm_t.tm_mon+1);
		if (tm_t.tm_mday>max_day)
		{
			tm_t.tm_mday = max_day;

		}
        time_t ret_time = mktime( &tm_t );
        if (-1 == ret_time)
        {
            printf("logic erro, mon=%d", tm_t.tm_mon);//为什么失败？
            return;
        }
        m_offset = m_offset + (ret_time - m_sec);
        Refresh();
    }

    void SysTime::AddYearOffset( unsigned int year )
    {
        Refresh();
        tm tm_t;
        localtime_r(&m_sec, &tm_t);
        tm_t.tm_year += year;
        time_t ret_time = mktime( &tm_t );
        if (-1 == ret_time)
        {
            printf("logic erro, year=%d", tm_t.tm_year);//为什么失败？
            return;
        }
        m_offset = m_offset + (ret_time - m_sec);
        Refresh();
    }

	time_t SysTime::GetCurDayStart( time_t sec )
	{
		tm _tm;
		localtime_r( &sec, &_tm);  
		_tm.tm_hour = 0;
		_tm.tm_min = 0;
		_tm.tm_sec = 0;
		time_t ret =  mktime( &_tm );
		if (-1 == ret)
		{
			printf("logic error, sec= %ld", sec);
			return 0;
		}
		return ret;
	}

	time_t SysTime::GetCurWeekStart( time_t sec )
	{
		tm _tm;
		localtime_r( &sec, &_tm);  
		_tm.tm_hour = 0;
		_tm.tm_min = 0;
		_tm.tm_sec = 0;
		time_t cur_day_start = mktime( &_tm );
		if (-1 == cur_day_start)
		{
			printf("logic error, sec= %ld", sec);
			return 0;
		}
		return cur_day_start - _tm.tm_wday * 3600*24;
	}

	time_t SysTime::GetCurMonStart( time_t sec )
	{
		tm _tm;
		localtime_r( &sec, &_tm);  
		_tm.tm_hour = 0;
		_tm.tm_min = 0;
		_tm.tm_sec = 0;
		_tm.tm_mday = 1;
		time_t cur_day_start = mktime( &_tm );
		if (-1 == cur_day_start)
		{
			printf("logic error, sec= %ld", sec);
			return 0;
		}
		return cur_day_start;
	}

	time_t SysTime::GetCurYearStart( time_t sec )
	{
		tm _tm;
		localtime_r( &sec, &_tm);  
		_tm.tm_hour = 0;
		_tm.tm_min = 0;
		_tm.tm_sec = 0;
		_tm.tm_mon = 0;
		_tm.tm_mday = 1;

		time_t cur_day_start = mktime( &_tm );
		if (-1 == cur_day_start)
		{
			printf("logic error, sec= %ld", sec);
			return 0;
		}
		return cur_day_start;

	}



    Timer::State Timer::state()
    {
        return m_state;
    }

    void Timer::start()
    {
        m_start=g_time.Sec();
        m_state = TIMER_S_RUN;
    }

    void Timer::pause()
    {
        if(TIMER_S_RUN == m_state )
        {
            m_pause=g_time.Sec();
            m_state = TIMER_S_PAUSE;
        }
    }

    void Timer::resume()
    {
        if(TIMER_S_PAUSE == m_state )
        {
            time_t cur_sec = g_time.Sec();
            assert(cur_sec>=m_pause);
            m_start += ( cur_sec - m_pause ); //加上暂停时间
            m_state = TIMER_S_RUN;
        }
    }

    time_t Timer::peerCurrentTimer() const
    {
        if(TIMER_S_RUN == m_state)
        {
            return g_time.Sec() - m_start;
        }
        else if(TIMER_S_PAUSE == m_state )
        { 
            return m_pause-m_start;
        }
        return time_t();
    }

    time_t Timer::end()
    {
        if(TIMER_S_RUN == m_state)
        {
            m_state = TIMER_S_NULL;
            return g_time.Sec() - m_start;
        }
        else if(TIMER_S_PAUSE == m_state )
        { //暂停，先恢复
            m_state = TIMER_S_NULL;
            return m_pause-m_start;
        }
        return 0;
    }

}