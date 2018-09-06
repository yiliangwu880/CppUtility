

#include "friend.h"
#include "../utility/memoryStream.h"
#include "../utility/misc.h"


void OneWayRelationBase::addPoint( OneWayRelationBase *pOther )
{
	uint32 myId = this->id();
	if (pOther->id() == myId)
	{
		return;
	}
	if(!m_setPoint.insert(pOther).second)
	{
		return;
	}
	pOther->addBePoint(this);
}

void OneWayRelationBase::delPoint( OneWayRelationBase *pOther )
{
	if(0==m_setPoint.erase(pOther))
	{
		return;
	}
	pOther->delBePoint(this);
}

const OneWayRelationBase::SetPoint & OneWayRelationBase::getSetPoint() const
{
	return m_setPoint;
}

const OneWayRelationBase::SetPoint & OneWayRelationBase::getSetBePoint() const
{
	return m_setBePoint;
}

void OneWayRelationBase::addBePoint( OneWayRelationBase *pOther )
{
	m_setBePoint.insert(pOther);
}

void OneWayRelationBase::delBePoint( OneWayRelationBase *pOther )
{
	m_setBePoint.erase(pOther);
}

void OneWayRelationBase::serialize( Memory &out ) const
{
	uint32 len = m_setPoint.size();
	out << len;
	CONST_FOR_IT(SetPoint, m_setPoint)
	{
		out << (*it)->id();
	}

	len = m_setBePoint.size();
	out << len;
	CONST_FOR_IT(SetPoint, m_setBePoint)
	{
		out << (*it)->id();
	}
}

void OneWayRelationBase::unserialize( const Memory &in )
{
	bool isMiss = false;

	uint32 len = 0;
	in >> len;
	for (uint32 i=0; i<len; ++i)
	{
		uint32 id = 0;
		in >> id;
		OneWayRelationBase *pOther = getObjById(id);
		if (NULL != pOther)
		{
			m_setPoint.insert(pOther);
		}
		else
		{
			isMiss = true;
		}
	}

	in >> len;
	for (uint32 i=0; i<len; ++i)
	{
		uint32 id = 0;
		in >> id;
		OneWayRelationBase *pOther = getObjById(id);
		if (NULL != pOther)
		{
			m_setBePoint.insert(pOther);
		}
		else
		{
			isMiss = true;
		}
	}

	if (isMiss)
	{
		in.setError();
	}
}
