/*
brief: 
 特点：
	 不可以从排行榜删除成员排名
	 排名数值从0开始
 注意：
	用户不要修改成员的排名 （Member::m_score_rank）
	成员分数变化时，调用RealTimeRankMgr::UpdateMemberRank


使用例子：

class Member
{
public:
Member(uint32 id, int64 score = 0, int64 lv = 0)
:m_score(score)
, m_score_rank(0)
, m_lv(0)
, m_lv_rank(0)
, m_id(id)
{
}
int64 GetScore(RankType type)
{
switch (type)
{
default:
LOG_ERROR("unknow rank type =%d", (int)type);
assert(false);
return 0;
case RankType::LV:		return m_lv;
case RankType::SCORE:	return m_score;
}
}

int32 GetRankIdx(RankType type)
{
switch (type)
{
default:
LOG_ERROR("unknow rank type =%d", (int)type);
assert(false);
return 0;
case RankType::LV:		return m_lv_rank;
case RankType::SCORE:	return m_score_rank;
}
}
void SetRankIdx(RankType type, int32 idx)
{
switch (type)
{
default:
LOG_ERROR("unknow rank type =%d", (int)type);
assert(false);
break;
case RankType::LV:		 m_lv_rank = idx; break;
case RankType::SCORE:	 m_score_rank = idx; break;
}
}
uint64 GetId(){ return m_id; }
public:
int64 m_score; int32 m_score_rank;
int64 m_lv; int32 m_lv_rank;
uint32 m_id;
};


main()
{
	RealTimeRankMgr<Member> g_mgr({ RankType::SCORE, RankType::LV });

	//初始化，例如读库
	/////////////////////////////////
	for (uint32 i = 0; i < ARRAY_LEN; ++i)
	{
		g_mgr.AddMemberNoRank(Member(i + 1, 10 + i));
	}
	g_mgr.Sort();
	/////////////////////////////////

	//运行过程，加新成员,或者成员改变分数
	g_mgr.AddMember(Member(1, 1131, 334));
	g_mgr.AddMember(Member(2, 1134, 335));

	{
	Member *p = g_mgr.GetMember(1);
	P->m_score+=1;
	mgr.UpdateMemberRank(p->m_id, RankType::SCORE);
	}

	{
	Member *p = g_mgr.GetMember(2);
	P->m_lv+=1;
	mgr.UpdateMemberRank(p->m_id, RankType::LV);
	}
}
*/

#pragma once
#include "stdafx.h"
#include <vector>
#include <algorithm>

#define  RANK_DEBUG

//新排行榜需要添加新的
enum class RankType
{
	SCORE=0,
	LV = 1,
};



template<class Member>
class RealTimeRankMgr;

template<class Member>
class RealTimeRank
{
	friend class RealTimeRankMgr<Member>;
public:
	typedef std::vector<Member *> VecMember;

public:
	RealTimeRank(RankType rank_type);

	Member *GetMember(uint32 rank) const;
	const VecMember &GetAllMember() const{ return m_vec_member; }
	RankType GetRankType() const{ return m_rank_type; }

private:
	void AddMember(Member &member);
	//当成员分数变化时，调用
	void UpdateMemberRank(Member &member);


	void AddMemberNoRank(Member &member);
	void Sort(); //重新排序

private:
	VecMember m_vec_member;
	RankType m_rank_type;
};

template<class Member> 
class RealTimeRankMgr
{
public:
	using Id_2_Member = std::map<uint64, Member>;
	using RTR = RealTimeRank<Member>;
	using Type_2_Tank = std::map<RankType, RTR>;

public:
	RealTimeRankMgr(const std::vector<RankType> &vec);
	//加成员，并加入排名
	bool AddMember(Member &member); 
	//更新指定成员排名
	void UpdateMemberRank(uint64 id, RankType type);

	const RealTimeRank<Member> *GetRank(RankType type) const;

	Member *GetMember(uint64 id) ;

	//加成员，不给排名
	bool AddMemberNoRank(Member &&member);
	void Sort(); //全部排行榜重新排序

private:
	Id_2_Member m_id_2_member;
	Type_2_Tank m_type_2_rank;
};

template<class Member>
RealTimeRankMgr<Member>::RealTimeRankMgr(const std::vector<RankType> &vec)

{
	for (const auto &v : vec)
	{
		m_type_2_rank.emplace(v, RTR(v));
	}
}


template<class Member>
bool RealTimeRankMgr<Member>::AddMemberNoRank(Member &&member)
{
	uint64 member_id = member.GetId();
	std::pair<typename Id_2_Member::iterator, bool> ret = m_id_2_member.emplace(member_id, member);
	if (!ret.second)
	{
		return false;
	}

	Member *pMember = &(ret.first->second);
	for (auto &v : m_type_2_rank)
	{
		v.second.AddMemberNoRank(*pMember);
	}
	return true;
}

template<class Member>
void RealTimeRankMgr<Member>::Sort()
{
	for (auto &v : m_type_2_rank)
	{
		RTR &rtr = v.second;
		rtr.Sort();
	}
}


template<class Member>
bool RealTimeRankMgr<Member>::AddMember(Member &member)
{
	uint64 member_id = member.GetId();
	std::pair<typename Id_2_Member::iterator, bool> ret = m_id_2_member.emplace(member_id, member);
	if (!ret.second)
	{
		return false;
	}

	Member *pMember = &(ret.first->second);
	for (auto &v : m_type_2_rank)
	{
		auto &rank = v.second;
		rank.AddMember(*pMember);
	}
	return true;
}

template<class Member>
void RealTimeRankMgr<Member>::UpdateMemberRank(uint64 id, RankType type)
{
	auto it = m_id_2_member.find(id);
	if (it == m_id_2_member.end())
	{
		LOG_ERROR("find member fail. id=%lu", id);
		return;
	}

	auto rank_it = m_type_2_rank.find(type);
	if (rank_it == m_type_2_rank.end())
	{
		LOG_ERROR("find rank type fail. %d", (int)type);
		return;
	}
	rank_it->second.UpdateMemberRank(it->second);
}

template<class Member>
const RealTimeRank<Member> * RealTimeRankMgr<Member>::GetRank(RankType type) const
{
	auto rank_it = m_type_2_rank.find(type);
	if (rank_it == m_type_2_rank.end())
	{
		return nullptr;
	}
	return &rank_it->second;
}

template<class Member>
Member *  RealTimeRankMgr<Member>::GetMember(uint64 id)
{
	auto iter = m_id_2_member.find(id);
	if (iter == m_id_2_member.end())
	{
		return nullptr;
	}
	return &(iter->second);
}


template<class Member>
RealTimeRank<Member>::RealTimeRank(RankType rank_type)
:m_rank_type(rank_type)
{
}

template<class Member>
void RealTimeRank<Member>::AddMember(Member &member)
{
	m_vec_member.push_back(&member);
	member.SetRankIdx(m_rank_type, m_vec_member.size() - 1);
	UpdateMemberRank(member);
}

template<class Member>
void RealTimeRank<Member>::UpdateMemberRank(Member &member)
{
	int32 rank = member.GetRankIdx(m_rank_type);
	{//上升排名
		typename VecMember::reverse_iterator cur_it = m_vec_member.rend() - rank - 1; //当前更新成员迭代器
		typename VecMember::reverse_iterator beginIter = m_vec_member.rend() - rank; //oldIter-1; 也就是当前排名前一名
		typename VecMember::reverse_iterator resultIter = m_vec_member.rend();
		int64 update_score = (*cur_it)->GetScore(m_rank_type);
		for (auto it = beginIter; it != m_vec_member.rend(); ++it)
		{
			if (update_score <= (*it)->GetScore(m_rank_type)) //找到比我前一名排名
			{
				resultIter = it;
				break;
			}
		}

		if (resultIter != beginIter)//有变化
		{
			member.SetRankIdx(m_rank_type, member.GetRankIdx(m_rank_type) + (beginIter - resultIter));
			for (auto it = beginIter; it != resultIter; ++it)
			{
				Member &m = *(*it);
				m.SetRankIdx(m_rank_type, m.GetRankIdx(m_rank_type) + 1);
			}
			rotate(cur_it, beginIter, resultIter);
#ifdef RANK_DEBUG
			std::cout << "move front, rank " << rank << " to rank " << rank - (resultIter - beginIter) << std::endl;
#endif

			return;
		}
	}

	{//下降排名

		typename VecMember::iterator cur_it = m_vec_member.begin() + rank;
		typename VecMember::iterator beginIter = cur_it + 1; //也就是当前排名后一名
		typename VecMember::iterator resultIter = m_vec_member.end();
		int64 update_score = (*cur_it)->GetScore(m_rank_type);
		for (auto it = beginIter; it != m_vec_member.end(); ++it)
		{
			if (update_score >= (*it)->GetScore(m_rank_type)) //找到比我后一名排名
			{
				resultIter = it;
				break;
			}
		}
		if (resultIter != beginIter)//有变化
		{
			member.SetRankIdx(m_rank_type, member.GetRankIdx(m_rank_type) + (resultIter - beginIter));
			for (auto it = beginIter; it != resultIter; ++it)
			{
				Member &m = *(*it);
				m.SetRankIdx(m_rank_type, m.GetRankIdx(m_rank_type) - 1);
			}
			rotate(cur_it, beginIter, resultIter);
#ifdef RANK_DEBUG
			std::cout << "move back, rank " << rank << " to rank" << rank + (resultIter - beginIter) << std::endl;
#endif
		}
	}
}

template<class Member>
Member *  RealTimeRank<Member>::GetMember(uint32 rank) const
{
	if (rank >= m_vec_member.size())
	{
		LOG_ERROR("rank out of rang. %d", rank);
		return nullptr;
	}
	return m_vec_member[rank];
}


template<class Member>
void RealTimeRank<Member>::AddMemberNoRank(Member &member)
{
	m_vec_member.push_back(&member);
	member.SetRankIdx(m_rank_type, -1);
}

template<class Member>
void RealTimeRank<Member>::Sort()
{
	RealTimeRank<Member> &rtr = *this;
	auto pred = [&rtr](Member *lh, Member *rh) -> bool
	{
		return lh->GetScore(rtr.m_rank_type) > rh->GetScore(rtr.m_rank_type);
	};
	std::sort(m_vec_member.begin(), m_vec_member.end(), pred);

	for (uint32 i = 0; i < m_vec_member.size(); ++i)
	{
		m_vec_member[i]->SetRankIdx(m_rank_type, i);
	}
}