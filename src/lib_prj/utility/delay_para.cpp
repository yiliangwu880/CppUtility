

#include "delay_para.h"
#include "misc.h"


DelayParaMgr::DelayParaMgr()
:m_time_out_sec(TIME_OUT_SEC)
{

}

void DelayParaMgr::OnTimer()
{
	time_t cur = 0;
	time(&cur);
	for (Para2Time::iterator it = m_para_2_time.begin(); it != m_para_2_time.end();)
	{
		if (cur - it->second > m_time_out_sec)
		{
			delete it->first;
			it=m_para_2_time.erase(it);
		} 
		else
		{
			++it;
		}
	}
}

BaseDelayPara * DelayParaMgr::FindPara(BaseDelayPara *para)
{
	Para2Time::iterator it = m_para_2_time.find(para);
	if (it != m_para_2_time.end())
	{
		return it->first;
	}
	return NULL;
}

uint32_t DelayParaMgr::GetParaCnt()
{
	return m_para_2_time.size();
}

void DelayParaMgr::DeletePara(BaseDelayPara *para)
{
	Para2Time::iterator it = m_para_2_time.find(para);
	if (it != m_para_2_time.end())
	{
		 delete it->first;
		 m_para_2_time.erase(it);
		 return;
	}
	L_ERROR("DeletePara fail, error call?");
}

void DelayParaMgr::SetTimeOutSec(uint64 sec)
{
	m_time_out_sec = sec;
}

BaseDelayPara::~BaseDelayPara()
{

}
