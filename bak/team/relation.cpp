

#include "relation.h"


void RelationBase::addPoint( RelationBase *pOther )
{
	if(!attach(pOther))
	{
		return;
	}
	pOther->attach(this);
}

void RelationBase::delPoint( RelationBase *pOther )
{
	if(!dettach(pOther))
	{
		return;
	}
	pOther->dettach(this);
}

bool RelationBase::attach( RelationBase *p )
{
	return m_setPoint.insert(p).second;
}

bool RelationBase::dettach( RelationBase *p )
{
	return 1==m_setPoint.erase(p);
}

void RelationBase::delAllPoint()
{
	for (SetPoint::iterator iter = m_setPoint.begin();
		iter!=m_setPoint.end(); )
	{
		(*iter)->dettach(this);
		m_setPoint.erase(iter++);
	}
}

bool RelationBase::isPoint( RelationBase *pOther ) const
{
	SetPoint::const_iterator iter=m_setPoint.find(pOther);
	return  iter != m_setPoint.end();
}

const RelationBase::SetPoint & RelationBase::getSetPoint() const
{
	return m_setPoint;
}

uint32 RelationBase::serialize( char *out ) const
{
	uint32 offset = 0;

	*(uint32 *)(out+offset)=m_setPoint.size();
	offset+=sizeof(uint32);
	for (SetPoint::const_iterator iter = m_setPoint.begin();
		iter!=m_setPoint.end(); ++iter)
	{
		*(uint32 *)(out+offset)=(*iter)->id();
		offset+=sizeof(uint32);
	}	
	return offset;
}

bool RelationBase::unserialize( const char *in )
{
	uint32 offset = 0;

	uint32 len = *(const uint32 *)(in+offset);
	offset+=sizeof(uint32);
	for (uint32 i=0; i<len; ++i)
	{
		uint32 id = *(const uint32 *)(in+offset);
		offset+=sizeof(uint32);
		RelationBase *pOther = getPointObj(id);
		if (NULL != pOther)
		{
			m_setPoint.insert(pOther);
			pOther->attach(this);
		}
	}
	return true;
}


void ApplyTeamBase::dimiss()
{
	for (SetApplyMember::iterator iter = m_setApplyMember.begin();
		iter!=m_setApplyMember.end(); )
	{
		(*iter)->beRefuse(this);
		m_setApplyMember.erase(iter++);
	}
	delAllPoint();
}

bool ApplyTeamBase::apply( ApplyMemberBase *pMember )
{
	uint32 memberID = pMember->id();
	if(0==memberID  //can,t use zero id
		|| 0==id()
		||isPoint(pMember)
		)
	{
		return false;
	}

	if (!m_setApplyMember.insert(pMember).second)
	{
		return false;
	}
	pMember->addApplyTeam(this);
	return true;	
}

bool ApplyTeamBase::applove( ApplyMemberBase *pMember )
{
	SetApplyMember::iterator iter = m_setApplyMember.find(pMember);
	if (iter==m_setApplyMember.end())
	{
		return false;
	}
	m_setApplyMember.erase(iter);
	pMember->beAppove(this);
	addPoint(pMember);
	return true;
}

void ApplyTeamBase::refuse( ApplyMemberBase *pMember )
{
	SetApplyMember::iterator iter = m_setApplyMember.find(pMember);
	if (iter==m_setApplyMember.end())
	{
		return;
	}
	m_setApplyMember.erase(iter);
	pMember->beRefuse(this);
}

bool ApplyTeamBase::isApplyMember( ApplyMemberBase *pMember )
{
	SetApplyMember::const_iterator iter=m_setApplyMember.find(pMember);
	return (iter != m_setApplyMember.end());
}

const ApplyTeamBase::SetApplyMember & ApplyTeamBase::getSetApplyMember() const
{
	return m_setApplyMember;
}

void ApplyTeamBase::beCancelApply( ApplyMemberBase *pMember )
{
	SetApplyMember::iterator iter=m_setApplyMember.find(pMember);
	if (iter!=m_setApplyMember.end())
	{
		m_setApplyMember.erase(iter);
	}
}

void ApplyTeamBase::attachApply( ApplyMemberBase *pMember )
{
	m_setApplyMember.insert(pMember);
}

bool ApplyMemberBase::isApply( ApplyTeamBase *pTeam )
{
	SetApplyTeam::iterator iter = m_setApplyTeam.find(pTeam);
	return iter!=m_setApplyTeam.end();
}

const ApplyMemberBase::SetApplyTeam & ApplyMemberBase::getSetApplyTeam() const
{
	return m_setApplyTeam;
}

void ApplyMemberBase::beAppove( ApplyTeamBase *pTeam )
{
	SetApplyTeam::iterator iter = m_setApplyTeam.find(pTeam);
	if (iter==m_setApplyTeam.end())
	{
		return;
	}
	m_setApplyTeam.erase(iter);
	onBeApplove(pTeam);
}

void ApplyMemberBase::addApplyTeam( ApplyTeamBase *pTeam )
{
	m_setApplyTeam.insert(pTeam);
	onApplyTeam(pTeam);
}

void ApplyMemberBase::beRefuse( ApplyTeamBase *pTeam )
{
	SetApplyTeam::iterator iter = m_setApplyTeam.find(pTeam);
	if (iter==m_setApplyTeam.end())
	{
		return;
	}
	m_setApplyTeam.erase(iter);
	onBeRefuse(pTeam);
}

uint32 ApplyMemberBase::serialize( char *out ) const
{
	uint32 offset = 0;

	*(uint32 *)(out+offset)=m_setApplyTeam.size();
	offset+=sizeof(uint32);
	for (SetApplyTeam::const_iterator iter = m_setApplyTeam.begin();
		iter!=m_setApplyTeam.end(); ++iter)
	{
		*(uint32 *)(out+offset)=(*iter)->id();
		offset+=sizeof(uint32);
	}	

	offset+=RelationBase::serialize(out+offset);
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
		ApplyTeamBase *pOther = dynamic_cast<ApplyTeamBase *>(getPointObj(id));
		if (NULL != pOther)
		{
			m_setApplyTeam.insert(pOther);
			pOther->attachApply(this);
		}
	}

	return RelationBase::unserialize(in+offset);
}

bool ApplyMemberBase::cancelApply( ApplyTeamBase *pTeam )
{
	SetApplyTeam::iterator iter=m_setApplyTeam.find(pTeam);
	if (iter!=m_setApplyTeam.end())
	{
		m_setApplyTeam.erase(iter);
		pTeam->beCancelApply(this);
		return true;
	}
	return false;
}

void ApplyMemberBase::cancelAllApply()
{
	for(SetApplyTeam::iterator iter=m_setApplyTeam.begin();
		iter!=m_setApplyTeam.end(); ++iter)
	{
		(*iter)->beCancelApply(this);
		m_setApplyTeam.erase(iter++);
	}
}


