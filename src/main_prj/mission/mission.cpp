
#include "mission.h"
#include "../utility/misc.h"
#include <algorithm>
#include <set>

using namespace std;

void MissionMgr::changeMissionState( uint32 condit_type, uint32 target_id, uint32 num )
{
	typedef set<MissionBase *> SetPoint;
	SetPoint set_p;
	for (VecReg::iterator iter = m_vecReg.begin();
		iter!=m_vecReg.end(); ++iter)
	{
		if (condit_type == iter->condit_type && target_id == iter->target_id)
		{
			iter->pMission->doChangeState(condit_type, target_id, num);
			set_p.insert(iter->pMission);
		}
	}
	CONST_FOR_IT(SetPoint, set_p)
	{
		afterUpdateState(*it);
	}
}

void MissionMgr::addMissionState( uint32 condit_type, uint32 target_id, uint32 num )
{
	typedef set<MissionBase *> SetPoint;
	SetPoint set_p;
	for (VecReg::iterator iter = m_vecReg.begin();
		iter!=m_vecReg.end(); ++iter)
	{
		if (condit_type == iter->condit_type && target_id == iter->target_id)
		{
			iter->pMission->doAddState(condit_type, target_id, num);
			set_p.insert(iter->pMission);
		}
	}
	CONST_FOR_IT(SetPoint, set_p)
	{
		afterUpdateState(*it);
	}
}

MissionBase *MissionMgr::createMission( uint32 mission_id )
{
	MissionBase *p = doCreateMission(mission_id);
	if (NULL == p)
	{
		return NULL;
	}
	regMissionElement(p);
	p->doInit();
	return p;
}

bool MissionMgr::destoryMission( uint32 mission_id )
{
	MissionBase *pMission = getMissionByID(mission_id);
	if (NULL == pMission )
	{
		return false;
	}
	unregMissionElement(pMission);
	doDestroyMission(mission_id);
	return true;
}

void MissionMgr::regMissionElement( MissionBase *pMission )
{
	typedef VecMissionItem VecElement;	
	VecElement vecElement;
	pMission->doGetAllElementId(vecElement);
	for (VecElement::const_iterator iter = vecElement.begin();
		iter!=vecElement.end(); ++iter)
	{
		m_vecReg.push_back(RegisterData(iter->typeID, iter->itemID, pMission));
	}
}
namespace{
struct MissionMgrFun
{
	const MissionBase *m_pMission;
	bool operator()(const MissionMgr::RegisterData &regData)
	{
		return m_pMission==regData.pMission;
	}
};
}
void MissionMgr::unregMissionElement( const MissionBase *pMission )
{
	MissionMgrFun fun;
	fun.m_pMission = pMission;
	VecReg::iterator eraseStart = remove_if(m_vecReg.begin(), m_vecReg.end(), fun);
	m_vecReg.erase(eraseStart, m_vecReg.end());
}




uint32 TriggerFind::getRelationId( ConditionType type, uint32 value )
{
	 ConditMapId &m = m_vec_table[type];
	ConditMapId::const_iterator it = m.find(value);
	if (it == m.end())
	{
		return 0;
	}
	return it->second;
}

void TriggerFind::addLib( ConditionType type, uint32 value, uint32 id )
{
	m_vec_table[type].insert(make_pair(value, id));
}

TriggerFind::TriggerFind()
{
	m_vec_table.resize(CT_MAX);
}
