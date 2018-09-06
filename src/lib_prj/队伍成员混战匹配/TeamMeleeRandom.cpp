

#include "TeamMeleeRandom.h"
#include <algorithm>
#include <cassert>

using namespace std;


void TeamMeleeRandom::clear()
{
	m_mapTeam.clear();
	m_lastNoMatchTeamId = 0;
	m_lastNoMatchMember.clear();
}

void TeamMeleeRandom::addCandidate( uint32 teamId, uint32 memberId )
{
	m_mapTeam[teamId].push_back(memberId);
}

bool TeamMeleeRandom::random( TeamMeleePair &teamMeleePair )
{
	if (!enableRandom())
	{
		return false;
	}
	teamMeleePair.clear();
	randomMemberOrderInTeam();
	preferRandomRemain(teamMeleePair);
	while(1)
	{
		VecId teamOrder;
		randomTeamOrder(teamOrder);
		if(teamOrder.size()<=1)
		{
			updateLastNoMatchMember();
			return true;
		}
		uint32 num = teamOrder.size()-1;
		for (uint32 i=0; i<num; i+=2)
		{
			uint32 teamId1 = teamOrder[i];
			uint32 teamId2 = teamOrder[i+1];
			teamMeleePair.add(teamId1, m_mapTeam[teamId1].back(), teamId2, m_mapTeam[teamId2].back());
			m_mapTeam[teamId1].pop_back();
			m_mapTeam[teamId2].pop_back();
			if (m_mapTeam[teamId1].empty())
			{
				m_mapTeam.erase(teamId1);
			}
			if (m_mapTeam[teamId2].empty())
			{
				m_mapTeam.erase(teamId2);
			}
		}
	}
	return true;
}

void TeamMeleeRandom::randomMemberOrderInTeam()
{
	MapTeam::iterator iter = m_mapTeam.begin();
	for (; iter!=m_mapTeam.end(); ++iter)
	{
		random_shuffle(iter->second.begin(), iter->second.end());
	}
}

void TeamMeleeRandom::preferRandomRemain( TeamMeleePair &teamMeleePair )
{
	if (0 == m_lastNoMatchTeamId)
	{
		return;
	}
	VecId teamOrder;
	randomTeamOrderByExcept(teamOrder, m_lastNoMatchTeamId);
	uint32 teamIndex = (uint32)-1; //make first teamIndex++ equal zero
	while(1)
	{
		if (m_lastNoMatchMember.empty())
		{
			m_lastNoMatchTeamId = 0;
			return;
		}
		if (teamOrder.empty())
		{
			return;
		}
		++teamIndex;
		if (teamIndex >= teamOrder.size())
		{
			teamIndex = 0;
		}
		
		uint32 otherTeamId = teamOrder[teamIndex];
		teamMeleePair.add(m_lastNoMatchTeamId, m_lastNoMatchMember.back(), otherTeamId, m_mapTeam[otherTeamId].back());
		m_lastNoMatchMember.pop_back();
		m_mapTeam[otherTeamId].pop_back();
		if (m_mapTeam[otherTeamId].empty())
		{
			m_mapTeam.erase(otherTeamId);
			teamOrder.erase(teamOrder.begin()+teamIndex);
		}
	}
}

void TeamMeleeRandom::randomTeamOrder( VecId &teamOrder ) const
{
	MapTeam::const_iterator iter = m_mapTeam.begin();
	for (; iter != m_mapTeam.end(); ++iter)
	{
		assert(!iter->second.empty());
		teamOrder.push_back(iter->first);
	}
	random_shuffle(teamOrder.begin(), teamOrder.end());
}

void TeamMeleeRandom::randomTeamOrderByExcept( VecId &teamOrder, uint32 exceptTeamId ) const
{
	MapTeam::const_iterator iter = m_mapTeam.begin();
	for (; iter!=m_mapTeam.end(); ++iter)
	{
		if (iter->first == exceptTeamId)
		{
			continue;
		}
		assert(!iter->second.empty());
		teamOrder.push_back(iter->first);
	}
	random_shuffle(teamOrder.begin(), teamOrder.end());
}

void TeamMeleeRandom::updateLastNoMatchMember()
{
	if (m_lastNoMatchMember.empty())
	{
		m_lastNoMatchTeamId = 0;
	}

	MapTeam::const_iterator iter = m_mapTeam.begin();
	if (iter != m_mapTeam.end())
	{
		m_lastNoMatchMember.insert(m_lastNoMatchMember.end(), iter->second.begin(), iter->second.end());
		m_lastNoMatchTeamId = iter->first;
	}
	m_mapTeam.clear();
}

bool TeamMeleeRandom::enableRandom() const
{
	if (m_mapTeam.empty())
	{
		return false;
	}
	if (1 == m_mapTeam.size() && m_lastNoMatchMember.empty())
	{
		return false;
	}
	if (1 == m_mapTeam.size())
	{
		MapTeam::const_iterator iter = m_mapTeam.begin();
		if (m_lastNoMatchTeamId == iter->first)
		{
			return false;
		}
	}
	return true;
}

void TeamMeleeRandom::getRemain( uint32 &teamId, VecId &vecMember ) const
{
	teamId = m_lastNoMatchTeamId;
	vecMember.insert(vecMember.end(), m_lastNoMatchMember.begin(), m_lastNoMatchMember.end());
	MapTeam::const_iterator iter = m_mapTeam.begin();
	if (iter!=m_mapTeam.end())
	{
		vecMember.insert(vecMember.end(), iter->second.begin(), iter->second.end());
	}

}

void TeamMeleePair::clear()
{
	m_vecPair.clear();	
}

void TeamMeleePair::add( uint32 teamId1, uint32 memberId1, uint32 teamId2, uint32 memberId2 )
{
	Pair pair;
	pair.first.teamId = teamId1;
	pair.first.memberId = memberId1;
	pair.second.teamId = teamId2;
	pair.second.memberId = memberId2;
	m_vecPair.push_back(pair);
}
