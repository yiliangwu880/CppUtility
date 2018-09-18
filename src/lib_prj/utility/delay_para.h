/*
brief: ��ʱ�������� �û���̬���ô�����ʼ���� ��ʱ���յ�������Ϣ�󣩣���ȡ�������Զ��ͷš�
���ӣ�
	create para, 
	init
	user req.

	//wait cb 
	 get para
	 user cb
	 auto release


	 //on timer
	 check an release timeout para
	
*/

#pragma once
#include "../utility/typedef.h"
#include <vector>
#include <map>
#include <utility>

struct DelayParaGuard;

class BaseDelayPara
{
public:
	virtual ~BaseDelayPara() = 0;

private:

};


class DelayParaMgr
{
	friend class DelayParaGuard;
	DelayParaMgr();
public:
	static DelayParaMgr &Instance()
	{
		static DelayParaMgr obj;
		return obj;
	}

	//���󴴽�
	template <typename DelayPara>
	DelayPara *CreatePara()
	{
		DelayPara *para = new DelayPara();
		if(NULL == para)
		{
			return NULL;
		}
		time_t cur=0;
		time(&cur);
		m_para_2_time.insert(std::make_pair(para, cur));
		return para;
	}
	//c++11������汾��������ĺ���
	template <typename DelayPara, typename... Args>
	DelayPara *CreatePara(Args&&... args)
	{
		DelayPara *para = new DelayPara(std::forward<Args>(args)...);
		if (NULL == para)
		{
			return NULL;
		}
		time_t cur = 0;
		time(&cur);
		m_para_2_time.insert(std::make_pair(para, cur));
		return para;
	}

	//����5�����һ��
	void OnTimer();

	DelayParaGuard *GetPara(BaseDelayPara *para);
private:
	void DeletePara(BaseDelayPara *para);

private:
	static const uint64 TIME_OUT_SEC = 10;  //����ʱ�䣬��

private:
	typedef std::map<BaseDelayPara *, uint64> Para2Time;

	Para2Time m_para_2_time; //���� 2 ����ʱ���
};


struct DelayParaGuard
{
	DelayParaGuard()
	:para(NULL)
	{}
	~DelayParaGuard()
	{
		if (NULL != para)
		{
			DelayParaMgr::Instance().DeletePara(para);
		}
	}

	BaseDelayPara *para;
};
