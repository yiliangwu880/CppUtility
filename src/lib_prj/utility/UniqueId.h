#pragma once
#include <list>
#include <map>
#include <vector>
#include <climits>
#include "../utility/cnt_typedef.h"
#include "../utility/typedef.h"
#include "../utility/noncopyable.h"



//0ֵ��ʾ��Чid, ռ������ڴ�Ϊ��ʷ���ʹ��id��
class UniqueID
{
public:
	UniqueID():m_max_index(0)
	{
	}
    void reset()
    {
        m_max_index = 0;
        m_vec_free.clear();
    }

	uint32 newId()
	{
		if (m_vec_free.empty())
		{
			if (m_max_index == (uint32)(-1))
			{
				return 0;
			}
			++m_max_index;
			return m_max_index;
		}
		uint32 id = m_vec_free.back();
		m_vec_free.pop_back();
		return id;
	}

    //ע�⣺ �������id,ֵ����ȷ���������û����ڲ�������֤��
	void delId(uint32 id)
	{
        if (0 == id)
        {
            return;
        }
		m_vec_free.push_back(id);
		//releaseMemory()
	}

	void releaseMemory()
	{
	/*
		����������������������Ԫ��==m_max_index-1.
		ɾ���������������ڴ�
		��Сm_max_index 
	*/
	}
private:
	uint32 m_max_index;   //��ʷ��߷����id��
	VecUint32 m_vec_free;
};


//Map�ṹ�������id�Σ��ŵ�ʡ�ڴ棬ȱ�����Ч�ʵ�
template <class IdType>
class MapUniqueID : public Noncopyable
{
private:
	typedef std::map<IdType, IdType> IdMapId;
	typedef std::pair<typename IdMapId::iterator, bool> InsertRet;
public:
	MapUniqueID()
	{
		m_id_m_id.insert(1, std::numeric_limits<IdType>::max()); //��C��׼��õ����ֵ
	}

	IdType newId()
	{
		if(isEmpty())
		{
			return 0;
		}
		typename IdMapId::iterator it = m_id_m_id.begin();
		IdType id = it->second;
		if (it->first == it->second)
		{
			m_id_m_id.erase(it);
		} 
		else
		{
			--(it->second);
		}
		return id;
	}

	void delId(IdType id)
	{
		merge(insertFree(id));
	}

private:
	//��ǰ�󣬺ϲ�
	void merge(InsertRet ret)
	{
		if (!ret.second)
		{
			return;
		}
		typename IdMapId::iterator it = ret.first;
		//back

		//front
		//unfinished


	}
	InsertRet insertFree(IdType id)
	{
		InsertRet ret;
		ret.second = false;
		typename IdMapId::iterator it = m_id_m_id.lower_bound(id);//get >= id position
		//�Һ�����
		//case illega
		if (id == it->first)
		{
			return ret;
		}
		if (it == m_id_m_id.end())
		{
			return m_id_m_id.insert(make_pair(id, id));
		}
		if (id+1 == it->first)
		{
			IdType second = it->second;
			m_id_m_id.erase(it);
			return m_id_m_id.insert(make_pair(id, second));
		} 

		//��ǰ����
		//case first position
		if (it == m_id_m_id.begin())
		{
			return m_id_m_id.insert(make_pair(id, id));
		}
		--it;
		//case illega
		if (id <= it->second)
		{
			return ret;
		}

		if (id == it->second+1)
		{
			it->second+=1;
			ret.second = true;
			return ret;
		}

		return m_id_m_id.insert(make_pair(id, id));
	}
	bool isEmpty()
	{
		if(1!=m_id_m_id.size())
		{
			return true;
		}
		typename IdMapId::const_iterator it = m_id_m_id.begin();
		if (it->first == it->second)
		{
			return false;
		}
		return true;
	}
private:

	IdMapId m_id_m_id; //����free id, ������
};


