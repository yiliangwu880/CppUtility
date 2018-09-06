/*
brief: ����������̨�����а� �ο��ã���������ȫ����
���ԣ�
��Ա֮�������������λ�á�
֧����key����rank���ҳ�Ա�� 
�����Դ�������ɾ����Ա����
������ֵ��0��ʼ
������Ա���ܣ�������˶�����ս��ͬ����ͻ��(�������ã�ս��ִ�� �ͽ������� �п��ܲ�ͬ�ͻ���������ͬ����ս�����᲻ͬ��)
����Ҫ�浵���浵������ RankMemberBase ��������ȥ��ɡ�
��Աid=0��ʾ������

use example:
	class ConcreteMember : public RankMemberBase
	{
	...
	}
	ExchangeRankMgr mgr(5);
	map<uint32, ConcreteMember> g_idMapMemeber;

	void addNewMember(const ConcreteMember &m)
	{
	pair<map<uint32, ConcreteMember>::iterator, bool> ret= g_idMapMemeber.insert(make_pair(m.id(), m));
	assert(ret.second);
	mgr.addNewMember(ret.first->second);
	}

	//////////////////////////////////////////////////////////////////////////
	1)������ʹ�ã�
	mgr.endFightExchange(memberN1,memberN2);


	2)����ʹ�ã�
	if(mgr.enableFight(n1,n2))
	{
		mgr.startFight(0,1);
	}

	�ȱ�Ľ���ս����������ã�
	if(isFightingPair(memberN1_rank, memberN2_rank))
	{
		gr.endFightExchange(memberN1,memberN2);
	}

*/

#pragma once
#include "stdafx.h"
#include <vector>

void testRankFun();

class RankMemberBase
{
public:
	friend class ExchangeRankMgr;
	virtual ~RankMemberBase(){};
	virtual uint32 rank() const=0;

protected:
	virtual void rank(uint32 _rank)=0;

};

class ExchangeRankMgr
{
private:
	typedef std::vector<RankMemberBase * > VecMember;
	typedef std::set<uint32> SetRank;
	struct RankPair  //���ζ�
	{
		uint32 r1,r2;
		RankPair(uint32 _r1, uint32 _r2)
		{
			r1 = _r1; r2=_r2;
		}
		bool operator<(const RankPair &other) const
		{
			if (r1==other.r1)
			{
				return r2<other.r2;
			}
			return r1<other.r1;
		}
	};
	typedef std::set<RankPair> SetRankPair;
	
public:
	explicit ExchangeRankMgr(uint32 reserve_num=2000);
	void addNewMember(RankMemberBase *p_member);
	RankMemberBase *getMember(uint32 rank) const;
	//ս������(����λ�ú���������)
	bool endFightExchange(uint32 active_rank, uint32 target_rank, bool is_win = true);

	//����ʹ�ú���(�����Ľ��̼���ս�������ʱ��ϳ�)
	//////////////////////////////////////////////////////////////////////////
	//����Ƿ���ս�����Ƿ���������
	bool enableFight(uint32 rank1, uint32 rank2) const;
	//ս����ʼ����ס,����ս�����ζԣ�
	void startFight(uint32 active_rank, uint32 target_rank);
	bool isFightingPair( uint32 active_rank, uint32 target_rank ) const;
	//////////////////////////////////////////////////////////////////////////

	void clear();
	//Ч�ʵ�
	bool erase(uint32 rank);
private:
	VecMember m_vec_member_p;
	SetRank m_lock_rank;						//��ס������(����ս����)
	SetRankPair m_set_rank_pair;				//����ս���е����ζ�
};