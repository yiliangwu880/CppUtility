//���cpuʱ�䣬��Ч�ʹ���
/*
ʹ�����ӣ�
{
	CPU_TIME_FUN
	your code ...
}
*/


#pragma once
#include <time.h>
#include <sys/time.h>
#include <string>


//��⴦����ʱ����ع���, ��Ҫֱ���������ӿڣ�����࣬�ð�ȫ���
class CpuTime
{
	static const unsigned MAX_NEST_NUM = 50;  //�����Ƕ��
public:
	static CpuTime &Instance()
	{
		static CpuTime d;
		return d;
	}
	//checkStart �� checkEnd ֧����Ƕʹ��
	//���Կ�ʼ 
	inline void checkStart()
	{
		if (m_idx >= (int)(MAX_NEST_NUM - 1))
		{
			return;
		}
		++m_idx;
		gettimeofday(&m_val[m_idx], NULL);
	}
	//���Խ���, ���ش�checkStart��checkEnd֮���cpuʱ�� ΢��
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
	int m_idx;			//��Ƕ����
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
//�������� CPU_TIME_START �꣬��Ҫƥ�䣬����д�����
//����ĳ����cpuʱ�䣬�� ��CPU_TIME_END ����ƥ��ʹ��
#define CPU_TIME_START CpuTime::Instance().checkStart();

#define CPU_TIME_END \
{\
	unsigned long long val = CpuTime::Instance().checkEnd(); \
}


//���Ը��д��뵽}��cpuʱ��
#define CPU_TIME_FUN TestCpuTimeGuard cpu_time_guard(__FILE__, __LINE__, __FUNCTION__);
#else
#define CPU_TIME_START 
#define CPU_TIME_END
#define CPU_TIME_FUN 
#endif //#ifdef DEBUG_TEST_CPU_TIME