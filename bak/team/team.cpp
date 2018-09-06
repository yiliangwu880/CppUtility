/*
组队相关
*/



#include "team.h"

TeamBase::TeamBase()
{}

bool TeamBase::add(TeamMemberBase *pMember)
{
	if(0==pMember->id()) //can,t use zero id
	{
		return false;
	}
	if(0==id()) //can,t use zero id
	{
		return false;
	}
	if (!attachMember(pMember))
	{
		return false;
	}
	pMember->joinTeam(this);
	return true;
}

void TeamBase::remove(uint32 id)
{
	MapType::iterator iter = m_map.find(id);
	if (iter==m_map.end())
	{
		return;
	}
	iter->second->leaveTeam();
	m_map.erase(iter);
}

void TeamBase::dismiss()
{
	MapType::iterator iter = m_map.begin();
	for (; iter!=m_map.end(); )
	{
		iter->second->leaveTeam();
		m_map.erase(iter++);
	}
}

bool TeamBase::isMember(TeamMemberBase *pMember)
{
	MapType::iterator iter = m_map.find(pMember->id());
	return iter!=m_map.end();
}

TeamBase::MemberIter TeamBase::beginMember()
{
	return m_map.begin();
}

TeamBase::MemberIter TeamBase::endMember()
{
	return m_map.end();
}

bool TeamBase::attachMember( TeamMemberBase *pMember )
{
	return m_map.insert(std::make_pair(pMember->id(), pMember)).second;
}

TeamBase::~TeamBase()
{
	dismiss();
}


TeamMemberBase::TeamMemberBase()
:m_pTeam(NULL)
{
}

void TeamMemberBase::joinTeam(TeamBase * pTeam)
{
	if (NULL!=m_pTeam)
	{
		m_pTeam->remove(id());
	}
	m_pTeam = pTeam;
	onJoinTeam(pTeam);
}

void TeamMemberBase::leaveTeam()
{
	m_pTeam = NULL;
	onLeaveTeam();
}

bool TeamMemberBase::attachTeam( TeamBase *pTeam )
{
	if(NULL != m_pTeam)
	{
		return false;
	}
	if(0==pTeam->id()) //can,t use zero id
	{
		return false;
	}
	m_pTeam = pTeam;
	return true;
}

uint32 TeamMemberBase::serialize( char *out ) const
{
	uint32 len = 0;
	if (NULL != m_pTeam)
	{
		*(uint32 *)(out+len)=m_pTeam->id();
	} 
	else
	{
		*(uint32 *)(out+len)=0;
	}
	len+=sizeof(uint32);
	return len;
}

bool TeamMemberBase::unserialize( const char *in )
{
	uint32 offset = 0;
	uint32 id = *(const uint32 *)(in+offset);
	offset+=sizeof(uint32);

	if (TeamBase *p=doFindTeam(id))
	{
		p->attachMember(this);
		attachTeam(p);
	}
	else
	{
		m_pTeam = NULL;
	}

	return true;
}

TeamMemberBase::~TeamMemberBase()
{
	if (NULL!=m_pTeam)
	{
		//m_pTeam->remove(id()); //类的析构是从派生类到基类，当调用继承层次中某一层次的类的析构函数时意味着其派生类部分已经析构掉，所以也不会呈现多态
	}
}

bool ApplyTeamBase::apply( ApplyMemberBase *pMember )
{
	if(0==pMember->id()) //can,t use zero id
	{
		return false;
	}
	if(0==id()) //can,t use zero id
	{
		return false;
	}
	if (isMember(pMember))
	{
		return false;
	}
	if (!attachApplyMember(pMember))
	{
		return false;
	}
	pMember->beApplyTeam(this);
	return true;
}

bool ApplyTeamBase::applove( ApplyMemberBase *pMember )
{
	SetType::iterator iter = m_mapApplyMembers.find(pMember);
	if (iter==m_mapApplyMembers.end())
	{
		return false;
	}
	m_mapApplyMembers.erase(iter);
	return TeamBase::add(pMember);
}

void ApplyTeamBase::refuse( ApplyMemberBase *pMember )
{
	SetType::iterator iter = m_mapApplyMembers.find(pMember);
	if (iter==m_mapApplyMembers.end())
	{
		return;
	}
	m_mapApplyMembers.erase(iter);
	pMember->beRefuse(this);
}

bool ApplyTeamBase::isApplyMember( ApplyMemberBase *pMember )
{
	SetType::iterator iter = m_mapApplyMembers.find(pMember);
	return iter!=m_mapApplyMembers.end();
}

ApplyTeamBase::ApplyMemberIter ApplyTeamBase::beginApplyMember()
{
	return m_mapApplyMembers.begin();
}

ApplyTeamBase::ApplyMemberIter ApplyTeamBase::endApplyMember()
{
	return m_mapApplyMembers.end();
}

bool ApplyTeamBase::attachApplyMember( ApplyMemberBase *pMember )
{
	return m_mapApplyMembers.insert(pMember).second;
}


ApplyTeamBase::~ApplyTeamBase()
{
	for (SetType::iterator iter=m_mapApplyMembers.begin();
		iter!=m_mapApplyMembers.end(); )
	{
		SetType::iterator temp = iter++;
		refuse(*temp);
	}
}

void ApplyMemberBase::beApplyTeam( ApplyTeamBase *pTeam )
{
	m_setApplyTeam.insert(pTeam);
	onApplyTeam(pTeam);
}

void ApplyMemberBase::beRefuse( ApplyTeamBase *pTeam )
{
	SetType::iterator iter = m_setApplyTeam.find(pTeam);
	if (iter==m_setApplyTeam.end())
	{
		return;
	}
	m_setApplyTeam.erase(iter);
	onBeRefuse(pTeam);
}

bool ApplyMemberBase::isApply( ApplyTeamBase *pTeam )
{
	SetType::iterator iter = m_setApplyTeam.find(pTeam);
	return iter!=m_setApplyTeam.end();
}

ApplyMemberBase::ApplyTeamIter ApplyMemberBase::beginApplyTeam()
{
	return m_setApplyTeam.begin();
}

ApplyMemberBase::ApplyTeamIter ApplyMemberBase::endApplyTeam()
{
	return m_setApplyTeam.end();
}

bool ApplyMemberBase::attachApplyTeam( ApplyTeamBase *pTeam )
{
	if(0==pTeam->id()) //can,t use zero id
	{
		return false;
	}
	return m_setApplyTeam.insert(pTeam).second;
}

uint32 ApplyMemberBase::serialize( char *out ) const
{
	uint32 offset = 0;

	*(uint32 *)(out+offset)=m_setApplyTeam.size();
	offset+=sizeof(uint32);
	for (SetType::const_iterator iter = m_setApplyTeam.begin();
		iter!=m_setApplyTeam.end(); ++iter)
	{
		*(uint32 *)(out+offset)=(*iter)->id();
		offset+=sizeof(uint32);
	}	

	offset+=TeamMemberBase::serialize(out+offset);
	return offset;
}

bool ApplyMemberBase::unserialize( const char *in )
{
	uint32 offset = 0;

	uint32 applyTeamLen = *(const uint32 *)(in+offset);
	offset+=sizeof(uint32);
	for (uint32 i=0; i<applyTeamLen; ++i)
	{
		uint32 id = *(const uint32 *)(in+offset);
		offset+=sizeof(uint32);

		if (ApplyTeamBase *p=doFindTeam(id))
		{
			p->attachApplyMember(this);
			attachApplyTeam(p);
		}
	}

	return TeamMemberBase::unserialize(in+offset);
}

ApplyMemberBase::~ApplyMemberBase()
{
	for (SetType::iterator iter = m_setApplyTeam.begin();
		iter!=m_setApplyTeam.end(); )
	{
		SetType::iterator temp = iter++;
		(*temp)->refuse(this);
	}
}
