
#include "mission.h"
#include "../utility/stlBoost.h"
#include "../utility/misc.h"
#include <assert.h>
#include "stdafx.h"

using namespace std;
using namespace wyl;

//use example
class Mission: public MissionBase
{
public:
	Mission(uint32 missionID, void *sys)
		:m_sys(sys)
		,m_id(missionID)
	{

	}
	uint32 serialize(char *out){ return 0; };
	bool unserialize(const char *in){ return false; };
	virtual bool isFinished() const {return false;}
protected:
private:

	virtual void doInit(){};
	virtual void doGetAllElementId(VecMissionItem &vecElementId) const
	{
		return;
	}
	virtual void doChangeState(uint32 typeID, uint32 itemID, uint32 num)
	{
		/*
		*/
	}
	virtual void doAddState(uint32 typeID, uint32 itemID, uint32 num)
	{

	};

private:
	void *m_sys;
	uint32 m_id;
};

class ConcreteMissionMgr : public MissionMgr
{
public:
	//由系统调用通知任务状态改变
	void finishFB(uint32 fbID)
	{	
		addMissionState(1,fbID, 1);
	}
	void changeItemNum(uint32 itemID, uint32 num)
	{
		changeMissionState(2,itemID,num);
	}
	void finishMission()
	{
		//get reward
		destoryMission(1);
	}

	uint32 serialize(char *out)
	{
		/*
		for write all missionID
		for m_mapMissions.begen().. m_mapMissions.end()
		{
		MissionBase *p = xx;	
		write p->id();
		p->serialize();
		}
		*/
		return 0;
	}
	bool unserialize(const char *in)
	{
		/*
		for read all missionID
		{
			MissionBase *p = reateMission(uint32 missionID);	read id
			p->unserialize();
		}
		*/
		return false;
	}
private:
	virtual MissionBase *doCreateMission(uint32 missionID)
	{
		Mission *p =  m_mapMissions.insert(missionID, Mission(1,0));		
		if (NULL == p)
		{
			return NULL;
		}
	}
	virtual void doDestroyMission(uint32 missionID)
	{
		m_mapMissions.erase(missionID)	;
	}
	virtual MissionBase *getMissionByID(uint32 missionID)
	{
		return m_mapMissions.find(missionID);
	}

	void f()
	{
		MapMission::const_iterator iter = m_mapMissions.begin();
	}
private:
	typedef EasyMap<uint32, Mission> MapMission;
	MapMission	m_mapMissions;
	void *sys;
};


//////////////////////test/////////////////////////////////////////////////////////////////////
namespace
{

enum
{
	T_FB___ID=1,
	T_ITEM_ID,
	T_EVENT_ID,
};


struct MissionTable 
{
	uint32 missionID;
	uint32 elementID[10];
}missionTable[]={
	{1,11},
	{2,21,22},
	{3,31,32,33},
};

struct ElementTable 
{
	uint32 id;
	uint32 typeID;
	uint32 itemID;
	uint32 maxNum;
}elementTable[]={
	{11,T_FB___ID,1,1},
	{21,T_ITEM_ID,1,1},
	{22,T_ITEM_ID,2,2},
	{31,T_ITEM_ID,2,2},
	{32,T_ITEM_ID,4,2},
	{33,T_EVENT_ID,1,1},
};

const ElementTable *findElementTable(uint32 id)
{
	for (uint32 i=0; i<ArrayLen(elementTable); ++i)
	{
		if(elementTable[i].id==id)
		{
			return &elementTable[i];
		}
	}
	return NULL;
}
//不需要接的任务
class Mission: public MissionBase
{
public:
	struct MyElement
	{
		uint32 id;
		uint32 typeID;
		uint32 itemID;
		uint32 currentNum;
		uint32 maxNum;
	};
	typedef vector<MyElement> VecMyElement;
public:
	Mission(uint32 missionID)
		:m_id(missionID)
	{
		bool bInit = false;
		for (uint32 i=0; i<ArrayLen(missionTable); ++i)
		{
			if (missionTable[i].missionID==missionID)
			{
				const MissionTable &table = missionTable[i];
				for (uint32 k=0; k<ArrayLen(table.elementID); ++k)
				{
					MyElement element;
					element.id = table.elementID[k];
					if (0==element.id)
					{
						break;
					}
					m_element.push_back(element);
					bInit = true;
				}
				break;
			}
		}
		assert(bInit);

		for (VecMyElement::iterator iter=m_element.begin();
			iter!=m_element.end(); ++iter)
		{
			const ElementTable *p=findElementTable(iter->id);
			if (NULL == p)
			{
				assert(0);
			}
			iter->typeID = p->typeID;
			iter->itemID = p->itemID;
			iter->maxNum = p->maxNum;
			iter->currentNum = 0;
		}
	}
	VecMyElement &vecElement()
	{
		return m_element;
	}
	virtual bool isFinished() const {return false;};
protected:
private:
	virtual void doInit(){};
	virtual void doGetAllElementId(VecMissionItem &vecElementId) const
	{
		for (VecMyElement::const_iterator iter=m_element.begin();
			iter!=m_element.end(); ++iter)
		{
			MissionItem elementID;
			elementID.typeID = iter->typeID;
			elementID.itemID = iter->itemID;
			vecElementId.push_back(elementID);
		}
	}
	virtual void doChangeState(uint32 typeID, uint32 itemID, uint32 num)
	{
		bool done = false;
		for (VecMyElement::iterator iter=m_element.begin();
			iter!=m_element.end(); ++iter)
		{
			if (typeID == iter->typeID && itemID == iter->typeID)
			{
				iter->currentNum = num;
				if (iter->currentNum>iter->maxNum)
				{
					iter->currentNum=iter->maxNum;
				}
				done = true;
			}
		}
		assert(done); //调用这函数对任务元素没有作用，可能任务管理基类出错。
	}
	virtual void doAddState(uint32 typeID, uint32 itemID, uint32 num)
	{
		bool done = false;
		for (VecMyElement::iterator iter=m_element.begin();
			iter!=m_element.end(); ++iter)
		{
			if (typeID == iter->typeID && itemID == iter->typeID)
			{
				iter->currentNum += num;
				if (iter->currentNum>iter->maxNum)
				{
					iter->currentNum=iter->maxNum;
				}
				done = true;
			}
		}
		assert(done); //调用这函数对任务元素没有作用，可能任务管理基类出错。
	};

private:
	uint32 m_id;
	VecMyElement m_element;
};

class ConcreteMissionMgr : public MissionMgr
{
public:
	virtual MissionBase *getMissionByID(uint32 missionID)
	{
		return m_mapMissions.find(missionID);
	}

private:
	virtual MissionBase *doCreateMission(uint32 missionID)
	{
		return m_mapMissions.insert(missionID, Mission(1));		
	}
	virtual void doDestroyMission(uint32 missionID)
	{
		m_mapMissions.erase(missionID)	;
	}

public:
	typedef EasyMap<uint32, Mission> MapMission;
	MapMission	m_mapMissions;
};

void finishAllMission(ConcreteMissionMgr &mgr)
{
	mgr.changeMissionState(T_FB___ID, 1, 1);
	mgr.changeMissionState(T_ITEM_ID, 1, 1);
	mgr.changeMissionState(T_ITEM_ID, 2, 2);
	mgr.changeMissionState(T_ITEM_ID, 2, 2);
	mgr.changeMissionState(T_ITEM_ID, 4, 2);
	mgr.changeMissionState(T_ITEM_ID, 1, 1);
}

void unfinishedAllMission(ConcreteMissionMgr &mgr)
{
	mgr.changeMissionState(T_FB___ID, 1, 0);
	mgr.changeMissionState(T_ITEM_ID, 1, 0);
	mgr.changeMissionState(T_ITEM_ID, 2, 0);
	mgr.changeMissionState(T_ITEM_ID, 2, 0);
	mgr.changeMissionState(T_ITEM_ID, 4, 0);
	mgr.changeMissionState(T_ITEM_ID, 1, 0);
}

void assertElementisStart(MissionBase *p)
{
	const Mission::VecMyElement &vecElement = dynamic_cast<Mission *>(p)->vecElement();
	for (Mission::VecMyElement::const_iterator iter=vecElement.begin();
		iter!=vecElement.end(); ++iter)
	{
		if(iter->currentNum!=0)
		{
			Info("错误，元素的currentNum不应该非0");
		}
	}
}

void assertElementisFinish(MissionBase *p)
{
	const Mission::VecMyElement &vecElement = dynamic_cast<Mission *>(p)->vecElement();
	for (Mission::VecMyElement::const_iterator iter=vecElement.begin();
		iter!=vecElement.end(); ++iter)
	{
		if(iter->currentNum!=iter->maxNum)
		{
			Info("错误，元素的currentNum不应该不等于maxNum");
		}
	}
}

//test all mission change state
void test1()
{
	Info("test1");
	ConcreteMissionMgr mgr;
	mgr.createMission(1);
	mgr.createMission(2);
	mgr.createMission(3);


	assertElementisStart(mgr.getMissionByID(1));
	finishAllMission(mgr);
	assertElementisFinish(mgr.getMissionByID(1));

	unfinishedAllMission(mgr);
	assertElementisStart(mgr.getMissionByID(2));
	finishAllMission(mgr);
	assertElementisFinish(mgr.getMissionByID(2));

	unfinishedAllMission(mgr);
	assertElementisStart(mgr.getMissionByID(3));
	finishAllMission(mgr);
	assertElementisFinish(mgr.getMissionByID(3));

}

//test destory finish mission
void test2()
{
	Info("test2");
	ConcreteMissionMgr mgr;
	mgr.createMission(1);
	mgr.createMission(2);
	mgr.createMission(3);

	mgr.changeMissionState(T_FB___ID, 1, 1); //finish 1
	assert(mgr.destoryMission(1));
	assert(2==mgr.m_mapMissions.size());
	mgr.changeMissionState(T_FB___ID, 1, 1); //finish 1
	mgr.createMission(1);

	assertElementisStart(mgr.getMissionByID(1));
}



}//end namespace

void testMission()
{
	test1();
	test2();

	Info("done");
}
