/*
�ο��ðɣ�һ�������Ƴɼ򵥵㡣 ����Ҫ�����֣���Ȼ�е㸴�ӣ����Ķ��Ե͡�
��Ƽ򵥵���磺player ������Ϣǰ������������player�������棬�Ȼص���ʱ����ͨ��playeridȥȡ���¡�

brief: ��ʱ�������� �û���̬���ô�����ʼ���� ��ʱ���յ�������Ϣ�󣩣���ȡ�������Զ��ͷš�
���ӣ�
	������������
	//���������ǣ����춯̬����
	class MyPara : public BaseDelayPara
	{
	public:
	MyPara(int p1, int p2)
	{
	a = p1; b = p2;
	}
	int a, b;
	};

	MyPara *p = DelayParaMgr::Instance().ConstructPara<MyPara>(1, 2);
	assert(p != NULL);
	send(p); --���͵����磬�Ȼص�


	//����ص�ʱ����
	DelayParaGuard<MyPara> guard(p); //�Զ��ͷ� p�ڴ�
	assert(guard.m_para != NULL);
	assert(guard.m_para->a == 1);


	 //��ʱ�������
	 DelayParaMgr::Instance().OnTimer();
	
*/

#pragma once
#include "../../svr_util/include/typedef.h"
#include <vector>
#include <map>
#include <utility>



class BaseDelayPara
{
public:
	virtual ~BaseDelayPara() = 0;

private:

};

//template <typename T> class DelayParaGuard; //pal����

class DelayParaMgr
{
	template <typename T> 
	friend class DelayParaGuard; //DelayParaGuard�������ʵ����, ��ΪDelayParaMgr����Ԫ

	DelayParaMgr();
public:
	static DelayParaMgr &Instance()
	{
		static DelayParaMgr obj;
		return obj;
	}

	//���󴴽�
	//template <typename DelayPara>
	//DelayPara *ConstructPara()
	//{
	//	DelayPara *para = new DelayPara();
	//	if(NULL == para)
	//	{
	//		return NULL;
	//	}
	//	time_t cur=0;
	//	time(&cur);
	//	m_para_2_time.insert(std::make_pair(para, cur));
	//	return para;
	//}
	//c++11������汾��������ĺ���
	template <typename DelayPara, typename... Args>
	DelayPara *ConstructPara(Args&&... args)
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
	//��ȡ���������ͷš�
	template <typename DelayPara>
	const DelayPara *PeerPara(BaseDelayPara *para)
	{
		BaseDelayPara *base = DelayParaMgr::Instance().FindPara(para);
		return dynamic_cast<DelayPara *>(base);
	}

	//����5�����һ��
	void OnTimer();
	uint32_t GetParaCnt();
	//һ�㲻���ã���Ĭ�ϵľ���
	void SetTimeOutSec(uint64 sec);

private:
	void DeletePara(BaseDelayPara *para);
	//���ز���ָ�룬 ʧ�ܷ���null
	BaseDelayPara *FindPara(BaseDelayPara *para);

private:
	static const uint64 TIME_OUT_SEC = 60*5;  //����ʱ�䣬��. ʱ����Գ��㣬���⶯̬�ڴ��ظ����ò���BUG�� �ص�������ȡ���ڴ治���Լ��ġ�
	typedef std::map<BaseDelayPara *, uint64> Para2Time;

	Para2Time m_para_2_time; //���� 2 ����ʱ���
	uint64 m_time_out_sec = 10;
};



template <typename DelayPara>
class DelayParaGuard
{
public:
	DelayParaGuard(BaseDelayPara *para)
	:m_para(NULL)
	{
		BaseDelayPara *base = DelayParaMgr::Instance().FindPara(para);
		m_para = dynamic_cast<DelayPara *>(base);
	}
	~DelayParaGuard()
	{
		if (NULL != m_para)
		{
			DelayParaMgr::Instance().DeletePara(m_para);
		}
	}

	DelayPara *m_para;
};
