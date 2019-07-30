/************************************************************************/
/*                                                                      
�����ο�������ˣ�ûʲô���ü�ֵ
*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include "../utility/singleton.h"
#include <map>
#include <vector>
#include <list>


class MissionBase;

//��������
struct MissionItem 
{
	uint32 typeID;
	uint32 itemID;
};
typedef std::vector<MissionItem> VecMissionItem;

//base class, manager active mission(have accepted mission).
class MissionMgr
{
public:
	struct RegisterData
	{
		RegisterData(uint32 _condit_type, uint32 _target_id, MissionBase *_pMission) 
			: condit_type(_condit_type)
			,target_id(_target_id)
			,pMission(_pMission)
		{};

		uint32 condit_type;
		uint32 target_id;
		MissionBase *pMission;
	};
public:
	virtual ~MissionMgr(){};
	MissionBase *createMission(uint32 missionID);	
	bool destoryMission(uint32 missionID);
	//�ı�����Ŀ������
	void changeMissionState(uint32 typeID, uint32 itemID, uint32 num);
	//��������Ŀ������
	void addMissionState(uint32 typeID, uint32 itemID, uint32 num);
	virtual MissionBase *getMissionByID(uint32 missionID)=0;
	//register mission. ������ʱ��Ҳ��Ҫȫ�������������һ�Ρ�
	void regMissionElement( MissionBase *pMission);	
private:
	virtual MissionBase *doCreateMission(uint32 missionID)=0;
	virtual void doDestroyMission(uint32 missionID)=0;
	//����״̬�����º����
	virtual void afterUpdateState(MissionBase *pMission){};
	void unregMissionElement(const MissionBase *pMission);
	
private:
	//�����������̫�࣬���Կ�����map�ṹ����Ч��.
	typedef std::vector<RegisterData> VecReg;

	VecReg m_vecReg;
};

class MissionBase
{
	friend class MissionMgr;
public:		

public:
	virtual ~MissionBase(){};
	virtual bool isFinished() const =0;
private:
	virtual void doInit()=0;
	virtual void doGetAllElementId(VecMissionItem &vecElementId) const=0;
	virtual void doChangeState(uint32 typeID, uint32 itemID, uint32 num)=0;
	//only can increase mission element.
	virtual void doAddState(uint32 typeID, uint32 itemID, uint32 num)=0;
};	


//����������������
/*
use example:
	for
	{
	//����������
	TriggerFind::obj().addLib(type,value,id);
	}

	//��ĳ�������͵���ֵʱ
	uint32 id = TriggerFind::obj().getRelationId(type,value);
	p_data = getTableById(id);
	if(��� p_data ���������������Ƿ���ϡ�)
	{
		�����ɹ���
	}

*/
enum ConditionType
{
	CT_LV,
	CT_MAX,
};
class TriggerFind: public Singleton<TriggerFind>
{

	friend class Singleton<TriggerFind>;
	typedef std::map<uint32, uint32> ConditMapId;
	typedef std::vector<ConditMapId> VecConditMapId;
public:
	TriggerFind();
	void addLib(ConditionType type, uint32 value, uint32 id);
	//return 0 when false
	uint32 getRelationId(ConditionType type, uint32 value);
protected:
private:
	VecConditMapId m_vec_table;		//type map ConditMapId
};

