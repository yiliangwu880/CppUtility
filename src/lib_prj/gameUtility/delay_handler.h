/*
brief:������ʱ�����߼��� ����:������������¼�
���ӣ�
	class MyTarget: public BaseDelayTarget
	class MyOpt: public BaseDelayOpt
	{
		virtual void Handle(BaseDelayTarget &target){....};
	};
	class MyOptMgr: public BaseDelayOptMgr
	{
	public:
		virtual BaseDelayTarget *FindTarget(uint64 target_id)
		{
			//ʵ�ֲ����ڴ�Ŀ��
		};			
		virtual void MissTarget(uint64 target_id)
		{
			//ʵ�֣����������  (����û�ж���ʱ����ã�
		};				
	};

	MyOptMgr mgr;
	//�����ɹ������ҵ�target����
	mgr.OptTarget(id, target);
	//����ʧ�ܵ���
	mgr.DelOpt(id);

	//�����������
	MyOptMgr mgr;
	MyOpt *opt =  mgr.CreateOpt<MyOpt>();
	//todo �û��Զ���opt��ʼ������
	mgr.AddOpt(target_id, opt);
*/

#pragma once
#include "../utility/typedef.h"
#include <vector>
#include <map>
#include <utility>

//����Ŀ��
class BaseDelayTarget
{
public:
	virtual ~BaseDelayTarget(){};
};

//�������
class BaseDelayOpt
{
public:
	virtual ~BaseDelayOpt(){};
	virtual void Handle(BaseDelayTarget &target) = 0;
};

class BaseDelayOptMgr
{
public:
	BaseDelayOptMgr();


	//���󴴽�BaseDelayOpt��������
	//�ص㣺�������Ҳ�����ڴ�й¶
	//ע�⣺���������ϵ���AddOpt�󣬲��ܵ���BaseDelayOptMgr��������Ա����Ȼ����һ��CreateOpt()������
	template <typename DeriveOpt>
	DeriveOpt *CreateOpt()
	{
		if (NULL != m_new_opt)
		{
			delete m_new_opt;
			printf("error call, call continue CreateOpt twice!!!\n");
			m_new_opt = NULL;
		}
		DeriveOpt *t = new DeriveOpt();
		if(NULL == t)
		{
			return NULL;
		}
		m_new_opt = t;
		return t;
	}
	//c++11������汾��������ĺ���
	template <typename DeriveOpt, typename... Args>
	DeriveOpt *CreateOpt(Args&&... args)
	{
		if (NULL != m_new_opt)
		{
			delete m_new_opt;
			printf("error call, call continue CreateOpt twice!!!\n");
			m_new_opt = NULL;
		}
		DeriveOpt *t = new DeriveOpt(std::forward<Args>(args)...);
		if (NULL == t)
		{
			return NULL;
		}
		m_new_opt = t;
		return t;
	}

	void AddOpt( uint64 target_id, BaseDelayOpt *opt );			//��һ������.		(Ŀ���ҵ�����ִ�У����ھ͵ȵ���  HandleTarget �ٲ���)
	void OptTarget(uint64 target_id, BaseDelayTarget &target);	//��Ŀ������������
	void DelOpt(uint64 target_id);								//����ɾ��Ŀ�껺�����  (����ʧ�ܣ�Ŀ�겻��ʱ����)

	//for test use
	int GetOptNum(uint64 target_id);
private:
    virtual BaseDelayTarget *FindTarget(uint64 target_id)=0;			//ʵ�ֲ����ڴ�Ŀ��
	virtual void MissTarget(uint64 target_id) = 0;				//ʵ�֣����������  (����û�ж���ʱ����ã�

public:
	static const uint32 MAX_REQ_NUM_PER_TARGET = 3;  //ÿ��target��󻺴�������

private:
	typedef std::vector<BaseDelayOpt *> VecBaseDelayOpt;
    typedef std::map<uint64, VecBaseDelayOpt> Id2VecOpt;


	Id2VecOpt m_id_2_vec_opt;
	BaseDelayOpt *m_new_opt;		//new����opt���󣬼�����ʱ����ʱ������ת�Ƶ�m_vec_opt
	bool m_is_opting;				//true��ʾ����BaseDelayOptMgr::OptTarget������
};

