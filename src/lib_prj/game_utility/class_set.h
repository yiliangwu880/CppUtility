/*
没复用价值，供参考
未测试
brief:分类的集合，适合列子（飞船有锁，解锁，拥有3状态，不会有交集）
*/

#pragma once
#include "../utility/cnt_typedef.h"
#include "../utility/misc.h"
#include "../utility/memoryStream.h"
#include <set>
#include <vector>


//集合的状态
//enum ClassSetType
//{
//	CST_1,		
//	CST_2,		//不保存类型
//};

//参数2为不保存类型，也是枚举最大值
template<typename SetType, uint32 nosave_type>
class ClassSet
{
	typedef std::vector<SetUint32> VecSetId;
public:
	ClassSet();
	//效率函数
	void setState(uint32 id,  SetType old_type, SetType new_type);
	const SetUint32 *getSet(SetType type) const;
	bool isState(uint32 id, SetType type) const;

	//低效函数
	SetType getState(uint32 id) const;
	void setState(uint32 id,  SetType new_type);

	void read(Memory &out) const;
	void write(const Memory &in);
protected:
private:
	VecSetId m_all;
};

template<typename SetType, uint32 nosave_type>
void ClassSet<SetType, nosave_type>::write( const Memory &in )
{
	in >> m_all;
}

template<typename SetType, uint32 nosave_type>
void ClassSet<SetType, nosave_type>::read( Memory &out ) const
{
	out << m_all;
}

template<typename SetType, uint32 nosave_type>
bool ClassSet<SetType, nosave_type>::isState( uint32 id, SetType type ) const
{
	if (nosave_type == type)
	{
		if (type == getState(id))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const SetUint32 &set_id = m_all[type];
	if (set_id.find(id)!=set_id.end())
	{
		return true;
	}
	return false;
}

template<typename SetType, uint32 nosave_type>
ClassSet<SetType, nosave_type>::ClassSet()
{
	m_all.resize(nosave_type);
}

template<typename SetType, uint32 nosave_type>
SetType ClassSet<SetType, nosave_type>::getState( uint32 id ) const
{
	for (uint32 i=0; i<m_all.size(); ++i)
	{
		const SetUint32 &set_id = m_all[i];
		if (set_id.find(id)!=set_id.end())
		{
			return i;
		}
	}
	return nosave_type;
}

template<typename SetType, uint32 nosave_type>
const SetUint32 * ClassSet<SetType, nosave_type>::getSet( SetType type ) const
{
	if (nosave_type == type)
	{
		return NULL;
	}
	return &m_all[type];
}

template<typename SetType, uint32 nosave_type>
void ClassSet<SetType, nosave_type>::setState( uint32 id, SetType old_type, SetType new_type )
{
	if (old_type!=nosave_type)
	{
		m_all[old_type].erase(id);
	}
	if (new_type!=nosave_type)
	{
		m_all[new_type].insert(id);
	}
}

template<typename SetType, uint32 nosave_type>
void ClassSet<SetType, nosave_type>::setState( uint32 id, SetType new_type )
{
	setState(id, getState(id), new_type);
}



