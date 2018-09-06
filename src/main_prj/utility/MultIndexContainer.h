// 作者：伍一良

/************************************************************************/
/* 多容器共同保存系列元素                                                                     */
/************************************************************************/

#include <map>

template<typename Key, typename Value, typename MapType=std::map<Key, Value*> >
struct IndexManager
{
	bool add(Key key, Value value)
	{
		return m_map.insert(std::make_pair(key, value)).second;
	}

	void remove(Value value)
	{
		typename MapType::iterator iter = m_map.begin();
		for (; iter!=m_map.end(); )
		{
			if(value == iter->second)
			{
				m_map.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}

	void erase(Key key)
	{
		typename MapType::iterator iter = m_map.find(key);
		if (iter!=m_map.end())
		{
			m_map.erase(iter);
		}
	}

	Value find(Key key)
	{
		typename MapType::iterator iter = m_map.find(key);
		if (iter==m_map.end())
		{
			return NULL;
		}
		return iter->second;
	}

	//Fun prototype: void f(Value *);
	template<typename Fun>
	inline void for_each(Fun f)
	{
		typename MapType::iterator iter = m_map.begin();
		for (; iter!=m_map.end(); ++iter)
		{
			f(iter->second);
		}
	}

	MapType m_map;
};


