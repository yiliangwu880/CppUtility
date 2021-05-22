//检查cpu时间，查效率功能
/*
使用例子：
{
	CPU_TIME_FUN
	your code ...
}
*/


#pragma once
#include <time.h>
#include <sys/time.h>
#include <string>


//检测处理器时间相关功能, 不要直接用这个类接口，陷阱多，用安全宏吧
class CpuTime
{
	static const unsigned MAX_NEST_NUM = 50;  //最大内嵌数
public:
	static CpuTime &Instance()
	{
		static CpuTime d;
		return d;
	}
	//checkStart 和 checkEnd 支持内嵌使用
	//测试开始 
	inline void checkStart()
	{
		if (m_idx >= (int)(MAX_NEST_NUM - 1))
		{
			return;
		}
		++m_idx;
		gettimeofday(&m_val[m_idx], NULL);
	}
	//测试结束, 返回从checkStart到checkEnd之间的cpu时间 微妙
	inline unsigned long long checkEnd()
	{
		if (m_idx < 0)
		{
			return 0;
		}
		timeval val_end;
		gettimeofday(&val_end, NULL);
		unsigned long long t = (val_end.tv_sec - m_val[m_idx].tv_sec) * 1000000 + (val_end.tv_usec - m_val[m_idx].tv_usec);
		--m_idx;
		return t;
	}
private:
	CpuTime()
		:m_idx(-1)
	{
	}

private:
	timeval m_val[MAX_NEST_NUM];
	int m_idx;			//内嵌索引
};


class TestCpuTimeGuard
{
public:
	inline TestCpuTimeGuard(const char *file, int line, const char *fun)
		:m_file(file)
		, m_fun(fun)
		, m_line(line)
	{
		CpuTime::Instance().checkStart();
	}
	inline ~TestCpuTimeGuard()
	{
		unsigned long long val = CpuTime::Instance().checkEnd();
		//ServerLogSingleton::Instance()->WriteLog(LOG_LEVEL_DEBUG, m_file.c_str(), m_line, m_fun.c_str(), "cpu time=%lld", val);
	}
private:
	std::string m_file, m_fun;
	int m_line;
};

#ifdef DEBUG_TEST_CPU_TIME
//建议少用 CPU_TIME_START 宏，需要匹配，容易写错代码
//测试某区间cpu时间，和 宏CPU_TIME_END 必须匹配使用
#define CPU_TIME_START CpuTime::Instance().checkStart();

#define CPU_TIME_END \
{\
	unsigned long long val = CpuTime::Instance().checkEnd(); \
}


//测试改行代码到}的cpu时间
#define CPU_TIME_FUN TestCpuTimeGuard cpu_time_guard(__FILE__, __LINE__, __FUNCTION__);
#else
#define CPU_TIME_START 
#define CPU_TIME_END
#define CPU_TIME_FUN 
#endif //#ifdef DEBUG_TEST_CPU_TIME