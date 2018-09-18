

#include "delay_para.h"
#include "../utility/misc.h"


DelayParaMgr::DelayParaMgr()
{

}

void DelayParaMgr::OnTimer()
{
	time_t cur = 0;
	time(&cur);
	for (Para2Time::iterator it = m_para_2_time.begin(); it != m_para_2_time.end();)
	{
		if (cur-it->second > TIME_OUT_SEC)
		{
			it=m_para_2_time.erase(it);
		} 
		else
		{
			++it;
		}
	}
}

BaseDelayPara::~BaseDelayPara()
{

}
