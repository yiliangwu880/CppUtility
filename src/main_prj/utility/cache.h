/*
brief:�����ڴ�
һ��򵥵Ĳ���Ҫ�����֣�ֱ��bool���ͱ�ʾ�Ƿ�dirty�Ϳ��ԣ�new, del����Ҫʵʱ��
*/
#pragma once
#include "typedef.h"


enum CacheStateEnum
{
	CACHE_S_NEW,
	CACHE_S_DEL,
	CACHE_S_MOD,
	CACHE_S_CLEAR, //δ�޸ĵģ��ʹ浵һ��
	CACHE_S_NONE, //������

	//����Ϊ�ǻ���״̬
	CACHE_S_ERROR,  //��ʾ����״̬

};

//����ı䶯�����
enum CacheChangeStateRet
{
	CCSR_CHANGE,			//�ı�
	CCSR_CHANGE_INVALID,	//�ı䲢�һ�����Ч
	CCSR_ERROR,
};
class CacheState
{
public:
	//�¶���������״̬���½�������߶�������
	explicit CacheState(bool isNew = false)
	{
		if (isNew)
		{
			m_state = CACHE_S_NEW;
		}
		else
		{
			m_state = CACHE_S_CLEAR;
		}
	}
	//ע�⣺return CCSR_CHANGE_INVALIDʱӦ��ɾ�����档
	CacheChangeStateRet changeState(CacheStateEnum action);
	CacheStateEnum state() const;
	bool dirty() const;
	
private:
	CacheStateEnum m_state;
};

void dkdjdk()
{
	CacheState a;
	//a.m_state = CacheState::DEL; 
}