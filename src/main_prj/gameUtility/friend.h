/************************************************************************/
/*                    
���ο���û���ü�ֵ
brief:
author:YiliangWu 

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <set>

class Memory;

//����ָ�����ѹ�ϵ������A��B�������ѣ�B��һ����AΪ����
//���ö���ǰ����������id�������ж���
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
	SetPoint m_setPoint;			//����ָ���ϵ
	SetPoint m_setBePoint;			//������ָ���ϵ
};

