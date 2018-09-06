

#include "delay_handler.h"
#include "../utility/misc.h"


void BaseDelayOptMgr::OptTarget( uint64 target_id, BaseDelayTarget &target )
{
	if (m_is_opting)
	{
		return;//���������ڵݹ���ã��˳����ô�����һ�ε��ý����ĺ����ᴦ��
	}
	int cnt = 0;
	while (1)
	{
		m_is_opting = true;
		//////////////���Դ�����룬����û�����ɾ��//////////
		cnt++;
		if (cnt >100)
		{
			printf("error,  endless loop?\n");
		}
		////////////////////////////////////////
		Id2VecOpt::iterator it = m_id_2_vec_opt.find(target_id);
		if (it == m_id_2_vec_opt.end())
		{
			m_is_opting = false;
			return;
		}

		VecBaseDelayOpt vec_opt;
		vec_opt.swap(it->second);
		m_id_2_vec_opt.erase(it);

		FOR_IT(VecBaseDelayOpt, vec_opt)
		{
			BaseDelayOpt *opt = *it;
			if (NULL == opt)
			{
				printf("error, why save null point?\n");
				continue;
			}
			opt->Handle(target);//�������ܼ�������AddOpt����m_id_2_vec_opt[id]�ӳ�Ա
			delete opt;
		}
	}
	m_is_opting = false;
}

void BaseDelayOptMgr::AddOpt( uint64 target_id, BaseDelayOpt *opt  )
{
	if (NULL == opt
		||opt != m_new_opt
		)
	{
		printf("error para %p\n", opt);
		m_new_opt = NULL;
		return;
	}
	if (0 == target_id)
	{
		printf("error uin\n");
		m_new_opt = NULL;
		return;
	}

	VecBaseDelayOpt &vec_opt = m_id_2_vec_opt[target_id];
	if (vec_opt.size() >= MAX_REQ_NUM_PER_TARGET)
	{
		//�������̫����		
		printf("error, req is too more, id=%ld\n", target_id);
		delete m_new_opt;
		m_new_opt = NULL;
		return;
	}
	m_new_opt = NULL; 
	vec_opt.push_back(opt);

	if (BaseDelayTarget *pTarget = FindTarget(target_id))
	{
		OptTarget(target_id, *pTarget);
	}
	else
	{
		//req get target from db
		if (vec_opt.size()== 1) //��һ���������������
		{
			MissTarget(target_id);
		}
	}
}


BaseDelayOptMgr::BaseDelayOptMgr()
	:m_new_opt(NULL)
	, m_is_opting(false)
{

}

void BaseDelayOptMgr::DelOpt( uint64 target_id )
{
	Id2VecOpt::iterator it=m_id_2_vec_opt.find(target_id);
	if (it==m_id_2_vec_opt.end())
	{
		return;
	}
	VecBaseDelayOpt &vec_opt = it->second;
	FOR_IT(VecBaseDelayOpt, vec_opt)
	{
		BaseDelayOpt *opt = *it;
		if (NULL == opt)
		{
			printf("error, why save null point?\n");
			continue;
		}
		delete opt;
	}
	m_id_2_vec_opt.erase(it);
}

int BaseDelayOptMgr::GetOptNum( uint64 target_id )
{
	Id2VecOpt::iterator it=m_id_2_vec_opt.find(target_id);
	if (it==m_id_2_vec_opt.end())
	{
		return 0;
	}
	VecBaseDelayOpt &vec_opt = it->second;
	return vec_opt.size();
}
