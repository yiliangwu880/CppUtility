/*
brief: 竞技场，擂台，排行榜。 参考用，不合适完全复用
特性：
成员之间会做交换排名位置。
支持用key或者rank查找成员。 
不可以从排名榜删除成员排名
排名数值从0开始
有锁成员功能，避免多人对其挑战不同步冲突。(服务器用，战斗执行 和交换排名 有可能不同客户端请求相同对象战斗，会不同步)
不需要存档，存档工作让 RankMemberBase 的派生类去完成。
成员id=0表示机器人

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
	1)不加锁使用：
	mgr.endFightExchange(memberN1,memberN2);


	2)加锁使用：
	if(mgr.enableFight(n1,n2))
	{
		mgr.startFight(0,1);
	}

	等别的进程战斗结束后调用：
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
	struct RankPair  //名次对
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
	//战斗结束(交换位置后有锁解锁)
	bool endFightExchange(uint32 active_rank, uint32 target_rank, bool is_win = true);

	//加锁使用函数(比如别的进程计算战斗结果，时间较长)
	//////////////////////////////////////////////////////////////////////////
	//检查是否能战斗（是否有上锁）
	bool enableFight(uint32 rank1, uint32 rank2) const;
	//战斗开始（锁住,纪律战斗名次对）
	void startFight(uint32 active_rank, uint32 target_rank);
	bool isFightingPair( uint32 active_rank, uint32 target_rank ) const;
	//////////////////////////////////////////////////////////////////////////

	void clear();
	//效率低
	bool erase(uint32 rank);
private:
	VecMember m_vec_member_p;
	SetRank m_lock_rank;						//锁住的名次(正在战斗中)
	SetRankPair m_set_rank_pair;				//正在战斗中的名次对
};