

#include "stdafx.h"

#include "../time/sys_time.h"
#include "../time/alarm_clock.h"
//#include "../utility/easyTest.h"
#include   <stdio.h>      
#include   <stdio.h>      
#include   <time.h>   


using namespace std;
using namespace wyl;
namespace
{
    //测试SysTime偏移
    void test1()
    {
        //这代码测试出错，如果低概率相差一秒就正常。
        g_time.SetTimeOffset(0);
        g_time.Refresh();
        time_t a = g_time.Sec();
        g_time.SetTimeOffset(1);
        assert(a+1 == g_time.Sec());
        g_time.SetTimeOffset(100);
        assert(a+100 == g_time.Sec());
        g_time.SetTimeOffset(1);
        assert(a+1 == g_time.Sec());
        g_time.SetTimeOffset(0);
        assert(a == g_time.Sec());

    }

    static uint32 s_cnt_1 = 0;
    void TimeCBFun2(uint32 para1, void *para2)
    {
        assert(1==para1);
        assert((void *)2==para2);
        s_cnt_1++;
    }
	void test2()
	{
        g_time.SetTimeOffset(0);
        g_timer_cb.setTimer(1, TimeCBFun2, 1, (void*)2);
        assert(1 == g_timer_cb.GetTimeNum());
        g_time.SetTimeOffset(1);
        g_timer_cb.checkTimeOut();
        assert(0 == g_timer_cb.GetTimeNum());
        assert(1 == s_cnt_1);
        assert(!g_timer_cb.delTimer(0));
	}

    //测试回调里面用timer接口
    static uint32 s_cnt_3 = 0;
    static uint32 s_t3_id = 0;
    void TimeCBFun3(uint32 para1, void *para2)
    {
        assert(1==para1);
        assert((void *)0==para2);
        s_cnt_3++;
        assert(0!=s_t3_id);
        assert(!g_timer_cb.delTimer(s_t3_id)); 
        if (1 == s_cnt_3)
        {
            s_t3_id = g_timer_cb.setTimer(2, TimeCBFun3, 1);
            g_timer_cb.setTimer(2, TimeCBFun3, 1);
            g_timer_cb.setTimer(3, TimeCBFun3, 1);
            g_timer_cb.setTimer(4, TimeCBFun3, 1);
            g_time.AddTimerOffset(4);
            assert(0!=s_t3_id);
        }
        else if (5 == s_cnt_3)
        {
            s_t3_id = g_timer_cb.setTimer(9, TimeCBFun3, 1);
            g_time.AddTimerOffset(9);
            assert(0!=s_t3_id);
        }
    }
    void test3()
    {
        g_time.SetTimeOffset(0);
        s_t3_id = g_timer_cb.setTimer(1, TimeCBFun3, 1);
        assert(0!=s_t3_id);
        g_time.AddTimerOffset(1);
        assert(1 == g_timer_cb.GetTimeNum());
        g_timer_cb.checkTimeOut();
        assert(4 == g_timer_cb.GetTimeNum());
        g_timer_cb.checkTimeOut();
        g_timer_cb.checkTimeOut();
        assert(6 == s_cnt_3);
        assert(0 == g_timer_cb.GetTimeNum());
    }

    //测试timerid 
    static uint32 s_cnt_4 = 0;
    void TimeCBFun4(uint32 para1, void *para2)
    {
        s_cnt_4++;
    }
    void test4()
    {
        g_time.SetTimeOffset(0);
        uint32 id = g_timer_cb.setTimer(1, TimeCBFun4);
        assert(0!=id);
        assert(1 == g_timer_cb.GetTimeNum());
        assert(!g_timer_cb.delTimer(0));
        assert(g_timer_cb.delTimer(id));
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        assert(0 == g_timer_cb.GetTimeNum());
        assert(0 == s_cnt_4);

        uint32 id1 = g_timer_cb.setTimer(1, TimeCBFun4);
        g_timer_cb.setTimer(1, TimeCBFun4);
        assert(2 == g_timer_cb.GetTimeNum());
        assert(g_timer_cb.delTimer(id1));
        assert(1 == g_timer_cb.GetTimeNum());
    }

    //loop timer回调里面能删除timer
    static uint32 s_cnt_5 = 0;
    void TimeCBFun5(uint32 para1, void *para2)
    {
        s_cnt_5++;
    }
    void test5()
    {
        g_timer_cb.clear();
        g_time.SetTimeOffset(0);
        uint32 id = g_timer_cb.setLoopTimer(1, TimeCBFun5);
        assert(0!=id);
        assert(1 == g_timer_cb.GetTimeNum());
        assert(!g_timer_cb.delTimer(0));
        assert(g_timer_cb.delTimer(id));
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        assert(0 == g_timer_cb.GetTimeNum());
        assert(0 == s_cnt_5);

        g_time.SetTimeOffset(0);
        id = g_timer_cb.setLoopTimer(1, TimeCBFun5);
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        g_time.AddTimerOffset(2);
        g_timer_cb.checkTimeOut();
        g_time.AddTimerOffset(5);
        for (uint32 i=0; i<100; ++i)
        {
            g_timer_cb.checkTimeOut();
        }
        assert(1 == g_timer_cb.GetTimeNum());
        assert(9 == s_cnt_5);


        assert(g_timer_cb.delTimer(id));
        g_time.AddTimerOffset(9);
        g_timer_cb.checkTimeOut();

        assert(0 == g_timer_cb.GetTimeNum());
        assert(9 == s_cnt_5);
    }


    //测试clear()
    static uint32 s_cnt_6 = 0;
    void TimeCBFun6(uint32 para1, void *para2)
    {
        s_cnt_6++;
    }
    void test6()
    {
        g_time.SetTimeOffset(0);
        g_timer_cb.setTimer(1, TimeCBFun6);
        g_timer_cb.setLoopTimer(1, TimeCBFun6);
        assert(2 == g_timer_cb.GetTimeNum());
        g_timer_cb.clear();
        assert(0 == g_timer_cb.GetTimeNum());
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        g_time.AddTimerOffset(2);
        g_timer_cb.checkTimeOut();
        assert(0 == s_cnt_6);

        g_timer_cb.setTimer(1, TimeCBFun6);
        g_timer_cb.setLoopTimer(1, TimeCBFun6);
        g_time.AddTimerOffset(7);
        g_timer_cb.checkTimeOut();
        assert(2 == s_cnt_6);
    }

    //测试timer_id回收  //测试回调调用接口 setTimer //测试回调调用接口 setLoopTimer
    static uint32 s_cnt_7 = 0;
    void TimeCBFun7(uint32 para1, void *para2)
    {
        s_cnt_7++;
        uint32 id=0;
        if (1 == para1)
        {
            id = g_timer_cb.setTimer(1, TimeCBFun7);
            assert(1 == id);
            id = g_timer_cb.setTimer(1, TimeCBFun7, 2);
            assert(2 == id);
        }
        else if(2 == para1)
        {
            id = g_timer_cb.setLoopTimer(1, TimeCBFun7);
            assert(2 == id);
            id = g_timer_cb.setTimer(1, TimeCBFun7, 3);
            assert(1 == id);
        }
        else if(3 == para1)
        {
            id = g_timer_cb.setTimer(1, TimeCBFun7);
            assert(1 == id);

        }
    }
    void test7()
    {
        g_time.SetTimeOffset(0);
        g_timer_cb.clear();
        uint32 id =g_timer_cb.setTimer(1, TimeCBFun7, 1);
        assert(1 == id);

        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        assert(5 == s_cnt_7);
    }

    //测试回调调用接口 delTimer
    static uint32 s_cnt_8 = 0;
    static uint32 s_8_del_id1 = 0;
    static uint32 s_8_del_id2 = 0;
    void TimeCBFun8(uint32 para1, void *para2)
    {
        s_cnt_8++;
        if (1 == para1)
        {
            assert(0!=s_8_del_id2);
            assert(g_timer_cb.delTimer(s_8_del_id2));
            assert(0!=s_8_del_id1);
            assert(!g_timer_cb.delTimer(s_8_del_id1));
        }
    }
    void test8()
    {
        g_time.SetTimeOffset(0);
        g_timer_cb.clear();
        s_8_del_id1 = g_timer_cb.setTimer(1, TimeCBFun8, 1);
        s_8_del_id2 = g_timer_cb.setLoopTimer(1, TimeCBFun8, 2);
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();

        assert(2 == s_cnt_8);
    }

    //测试回调调用接口 clear();
    static uint32 s_cnt_9 = 0;
    void TimeCBFun9(uint32 para1, void *para2)
    {
        s_cnt_9++;
        if (1 == para1)
        {
            g_timer_cb.clear();
        }
    }
    void test9()
    {
        g_time.SetTimeOffset(0);
        g_timer_cb.clear();
        s_8_del_id1 = g_timer_cb.setLoopTimer(1, TimeCBFun9, 1);
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        g_time.AddTimerOffset(2);
        g_timer_cb.checkTimeOut();

        assert(1 == s_cnt_9);
        assert(0==g_timer_cb.GetTimeNum());
    }

    //测试数量溢出
    static uint32 s_cnt_10 = 0;
    void TimeCBFun10(uint32 para1, void *para2)
    {
        s_cnt_10++;
    }
    void test10()
    {
        g_time.SetTimeOffset(0);
        g_timer_cb.clear();
		printf("next line print \"timer num is over load\" is ok:\n");
        for (uint32 i=0; i<NUM_1W; ++i)
        {
            uint32 id = g_timer_cb.setLoopTimer(1, TimeCBFun10);
            if (0 == id)
            {
                break;
            }
        }
        assert(1000==g_timer_cb.GetTimeNum());
        g_timer_cb.delTimer(1);
        assert(999==g_timer_cb.GetTimeNum());
        g_time.AddTimerOffset(1);
        g_timer_cb.checkTimeOut();
        assert(999==s_cnt_10);

    }

    void TestSysTime()
    {
        assert(0 == SysTime::GetMonthPeriodCnt(0, 0));
        assert(1 == SysTime::GetMonthPeriodCnt(0, 1));
        assert(13 == SysTime::GetMonthPeriodCnt(1, 1));

        assert(SysTime::IsLeapYear(1972));
        assert(!SysTime::IsLeapYear(1970));
        assert(SysTime::IsLeapYear(2000));
        assert(!SysTime::IsLeapYear(2100));

        assert(29==SysTime::GetDaysInMonth(1972, 2));
        assert(28==SysTime::GetDaysInMonth(1970, 2));
        assert(29==SysTime::GetDaysInMonth(2000, 2));
        assert(28==SysTime::GetDaysInMonth(2100, 2));

        g_time.SetTimeOffset(0);
        g_time.Refresh();
        time_t real_sec = g_time.Sec();
        assert(0 != real_sec);

        //测试调试时间
        ///////////////////////////////////
        g_time.AddTimerOffset(0);
        const char *p1 = NULL;

        p1 = "2015-01-01 00-00-01";
        assert(g_time.SetTimeByStr(p1));
        assert(p1 == g_time.GetTimeStr());

        p1 = "1970-01-01 07-00-01";
        assert(!g_time.SetTimeByStr(p1));
        assert("2015-01-01 00-00-01" == g_time.GetTimeStr());
     
        p1 = "1970-01-01 08-00-01";
        assert(g_time.SetTimeByStr(p1));
        assert(p1 == g_time.GetTimeStr());
        g_time.AddTimerOffset(3600);
        assert("1970-01-01 09-00-01" == g_time.GetTimeStr());
        
        ///////////////////////////////////
        
        assert(g_time.SetTimeByStr("1972-02-01 08-00-01"));
        assert(g_time.IsLeapYear());
        assert(29==g_time.GetDaysInMonth());
        assert(g_time.SetTimeByStr("1970-02-01 08-00-00"));
        assert(!g_time.IsLeapYear());
        assert(28==g_time.GetDaysInMonth());
        assert(g_time.SetTimeByStr("2000-02-01 08-00-01"));
        assert(g_time.IsLeapYear());
        assert(29==g_time.GetDaysInMonth());
        assert(g_time.SetTimeByStr("2100-02-01 08-00-01"));
        assert(!g_time.IsLeapYear());
		assert(28==g_time.GetDaysInMonth());
		assert(g_time.SetTimeByStr("2010-02-01 08-00-01"));
		assert(!g_time.IsLeapYear());

        assert(g_time.SetTimeByStr("2100-01-01 00-00-00"));
        assert(31==g_time.GetDaysInMonth());
        assert(g_time.SetTimeByStr("2100-08-01 00-00-00"));
        assert(31==g_time.GetDaysInMonth());
        assert(g_time.SetTimeByStr("2100-09-01 00-00-00"));
        assert(30==g_time.GetDaysInMonth());
        assert(g_time.SetTimeByStr("2100-12-41 00-00-00"));//故意写41日，里面会调整
        assert(31==g_time.GetDaysInMonth());

        //测试getMonthPeriodCnt
        assert(g_time.SetTimeByStr("1970-01-01 08-00-00"));
        assert(1 == g_time.GetMonthPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-31 23-59-59"));;
        assert(1 == g_time.GetMonthPeriodCnt());
        assert(g_time.SetTimeByStr("1970-02-22 00-00-00"));
        assert(2 == g_time.GetMonthPeriodCnt());
        assert(g_time.SetTimeByStr("2015-12-22 00-00-00"));
        assert(552 == g_time.GetMonthPeriodCnt());

        //测试getDayPeriodCnt
        assert(g_time.SetTimeByStr("1970-01-01 08-00-00"));
        assert(0 == g_time.GetDayPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-04 00-00-00"));
        assert(0 == g_time.GetDayPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-04 23-59-59"));
        assert(0 == g_time.GetDayPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-05 00-00-00"));
        assert(1 == g_time.GetDayPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-06 00-00-00"));
        assert(2 == g_time.GetDayPeriodCnt());
        assert(g_time.SetTimeByStr("2015-01-06 00-00-00"));
        assert(16438 == g_time.GetDayPeriodCnt());
        assert(g_time.SetTimeByStr("2015-01-05 23-59-59"));
        assert(16437 == g_time.GetDayPeriodCnt());

        assert(g_time.SetTimeByStr("1970-01-04 00-00-00"));
        assert(0 == g_time.GetWeekPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-11 00-00-00"));
        assert(1 == g_time.GetWeekPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-17 23-59-59"));
        assert(1 == g_time.GetWeekPeriodCnt());
        assert(g_time.SetTimeByStr("1970-01-18 00-00-00"));
        assert(2 == g_time.GetWeekPeriodCnt());
        assert(g_time.SetTimeByStr("2015-12-20 00-00-00"));
        assert(2398 == g_time.GetWeekPeriodCnt());
        assert(g_time.SetTimeByStr("2015-12-19 23-59-59"));
        assert(2397 == g_time.GetWeekPeriodCnt());

    }

    void test11()
    {
        {
            g_time.SetTimeOffset(0);
            uint32 start_mon =  g_time.GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                g_time.AddMonthOffset(1);
            }
            uint32 end_mon = g_time.GetMonthPeriodCnt();
            assert(30==end_mon-start_mon);

		}
		{
			g_time.SetTimeOffset(0);
			uint32 start_mon =  g_time.GetMonthPeriodCnt();
			g_time.AddMonthOffset(32);
			uint32 end_mon = g_time.GetMonthPeriodCnt();
			assert(32==end_mon-start_mon);
		}
        {
            g_time.SetTimeOffset(0);
            uint32 start_mon =  g_time.GetMonthPeriodCnt();
            for (uint32 i=0; i<30; ++i)
            {
                g_time.AddMonthOffset(2);
            }
            uint32 end_mon = g_time.GetMonthPeriodCnt();
            assert(30*2==end_mon-start_mon);
        }
        {
            g_time.SetTimeOffset(0);
            uint32 start =  g_time.CurTm().tm_year;
            for (uint32 i=0; i<30; ++i)
            {
                g_time.AddYearOffset(2);
            }
            uint32 end = g_time.CurTm().tm_year;
            assert(30*2==end-start);
        }
    }

	void test12()
	{
		g_time.SetTimeOffset(0);
		g_time.Refresh();
		time_t real_sec = g_time.Sec();
		assert(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		g_time.AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "2016-05-05 00-00-01";
		assert(g_time.SetTimeByStr(p1));
		assert(p1 == g_time.GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2016-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2016-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2016-01-01 00-00-00");
		}

		p1 = "2015-05-05 00-00-00";
		assert(g_time.SetTimeByStr(p1));
		assert(p1 == g_time.GetTimeStr());

		{
			string str;
			time_t sec = SysTime::GetCurDayStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2015-05-05 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurWeekStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2015-05-03 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurMonStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2015-05-01 00-00-00");
		}
		{
			string str;
			time_t sec = SysTime::GetCurYearStart(g_time.Sec());
			assert(SysTime::TimeToTimeStr(sec, str));
			assert(str=="2015-01-01 00-00-00");
		}
	}
	void test13()
	{
		g_time.SetTimeOffset(0);
		g_time.Refresh();
		time_t real_sec = g_time.Sec();
		assert(0 != real_sec);

		//测试调试时间
		///////////////////////////////////
		g_time.AddTimerOffset(0);
		const char *p1 = NULL;

		p1 = "a";
		assert(!g_time.SetTimeByStr(p1));
		p1 = "2015-01-";
		assert(!g_time.SetTimeByStr(p1));
		p1 = "2015-01-01";
		assert(g_time.SetTimeByStr(p1));
		assert("2015-01-01 00-00-00" == g_time.GetTimeStr());
	}

}//end namespace

void test_wyl_time()
{
	string s;
	g_time.TimeToTimeStr(1472688000, s);
	g_time.TimeToTimeStr(1470467234, s);
	// time=    1472688000   cur=   1470467234
    TestSysTime();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
	test11();
	test12();
	test13();

    Info("wyl_time test done");
}