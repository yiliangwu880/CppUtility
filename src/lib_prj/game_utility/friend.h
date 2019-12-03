/************************************************************************/
/*                    
供参考，没复用价值
brief:
author:YiliangWu 

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <set>

class Memory;

//单向指定朋友关系，比如A把B当做好友，B不一定当A为好友
//调用读档前，必须先用id创建所有对象
class OneWayRelationBase //relation player Base
{
public:
	typedef std::set<OneWayRelationBase *> SetPoint; 
public:
	void addPoint(OneWayRelationBase *pOther);
	void delPoint(OneWayRelationBase *pOther);
	const SetPoint &getSetPoint() const;
	const SetPoint &getSetBePoint() const;
	void serialize(Memory &out) const;
	void unserialize(const Memory &in);
	virtual uint32 id() const=0;
private:
	void addBePoint(OneWayRelationBase *pOther);
	void delBePoint(OneWayRelationBase *pOther);
	virtual OneWayRelationBase *getObjById(uint32 id) const=0;

private:
	SetPoint m_setPoint;			//主动指向关系
	SetPoint m_setBePoint;			//被主动指向关系
};

