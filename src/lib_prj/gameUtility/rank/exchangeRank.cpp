/*
brief: 竞技场，擂台，排行榜。


*/
#include "exchangeRank.h"

using namespace std;

ExchangeRankMgr::ExchangeRankMgr(uint32 reserve_num)
{
	m_vec_member_p.reserve(reserve_num);
}

void ExchangeRankMgr::addNewMember( RankMemberBase *p_member )
{
	if (nullptr == p_member)
	{
		LOG_ERROR("nullptr point");
		return;
	}
	p_member->rank(m_vec_member_p.size());
	m_vec_member_p.push_back(p_member);
}

RankMemberBase *ExchangeRankMgr::getMember( uint32 rank ) const
{
	if (rank>=m_vec_member_p.size())
	{
		return nullptr;
	}
	return m_vec_member_p[rank];
}

bool ExchangeRankMgr::enableFight( uint32 rank1, uint32 rank2 ) const
{
	if(m_lock_rank.find(rank1) != m_lock_rank.end()
		||m_lock_rank.find(rank2) != m_lock_rank.end()
		)
	{
		return false;
	}
	if (rank1>=m_vec_member_p.size() || rank2>=m_vec_member_p.size())
	{
		return false;
	}
	return true;
}

void ExchangeRankMgr::startFight( uint32 active_rank, uint32 target_rank )
{
	if(!enableFight(active_rank, target_rank))
	{
		LOG_ERROR("can't fight");
		return;
	}
	m_lock_rank.insert(active_rank);
	m_lock_rank.insert(target_rank);
	m_set_rank_pair.insert(RankPair(active_rank,target_rank));
}

bool ExchangeRankMgr::endFightExchange( uint32 active_rank, uint32 target_rank, bool is_win )
{
	if ( active_rank>=m_vec_member_p.size() || target_rank>=m_vec_member_p.size() )
	{
		return false;
	}
	RankMemberBase *p_active = m_vec_member_p[active_rank];
	if ( NULL == p_active )
	{
		assert(false); 
		return false;
	}
	RankMemberBase *p_target = m_vec_member_p[target_rank];
	if (NULL == p_target)
	{
		assert(false);
		return false;
	}
	if (m_set_rank_pair.find(RankPair(active_rank, target_rank)) == m_set_rank_pair.end())
	{
		LOG_ERROR("two target havn't call startFight %d %d", active_rank, target_rank);
		return false;
	}
	if (m_lock_rank.find(active_rank) == m_lock_rank.end()
		|| m_lock_rank.find(target_rank) == m_lock_rank.end()
		)
	{
		assert(false);
		return false;
	}

	//excute
	if (is_win)
	{
		p_active->rank(target_rank);
		p_target->rank(active_rank);
		std::swap(m_vec_member_p[active_rank], m_vec_member_p[target_rank]);
	}

	m_lock_rank.erase(active_rank);
	m_lock_rank.erase(target_rank);
	m_set_rank_pair.erase(RankPair(active_rank,target_rank));
	return true;
}

void ExchangeRankMgr::clear()
{
	m_vec_member_p.clear();
	m_lock_rank.clear();
	m_set_rank_pair.clear();
}

bool ExchangeRankMgr::erase( uint32 rank )
{
	if (
		!m_lock_rank.empty()
		|| !m_set_rank_pair.empty()
		)
	{
		LOG_ERROR("have some lock member,can't erase member");
		return false;
	}
	if (rank>=m_vec_member_p.size())
	{
		LOG_ERROR("rank out of rang. %d", rank);
		return false;;
	}
	m_vec_member_p.erase(m_vec_member_p.begin()+rank);
	uint32 maxSize = m_vec_member_p.size();
	for(uint32 i=rank; i<maxSize; ++i)
	{
		m_vec_member_p[i]->rank(i);
	}
	return true;
}

bool ExchangeRankMgr::isFightingPair( uint32 active_rank, uint32 target_rank ) const
{
	if(m_set_rank_pair.find(RankPair(active_rank,target_rank)) != m_set_rank_pair.end())
	{
		return true;
	}
	return false;
}
