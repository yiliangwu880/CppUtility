/*
准备废弃了，boost有multi_index功能代替
brief:多个key 的map. 未测试
use example:
	MapType 必须有函数mainKey， subKey1， subKey2
	struct Data 
	{
		int mainKey(){return key1;};
		char subKey1(){return key2;};
		double subKey2(){return key3;};
		int key1;
		char key2;
		double key3;
		int  data;
	};
	MultKeyMap<Data, int, char, double> m;
	...


*/
#pragma once
#include "typedef.h"
#include <map>
#include <vector>



class NoneSubKey1
{
};

class NoneSubKey2
{
};


namespace std
{
	// TEMPLATE CLASS map

	template<typename T>
	class map<NoneSubKey1, T>
	{
	public:
		typedef std::pair<int, bool> PairIB;
		typedef int iterator;
		typedef const int const_iterator;
		typedef NoneSubKey1 key_type;
		T insert(NoneSubKey1 &key){return make_pair(1,true);}
		T find(NoneSubKey1 &key){return NULL;}
		T end(){return NULL;}
		void erase(NoneSubKey1 &key){};
	};
	template<typename T>
	class map<NoneSubKey2, T>
	{
	public:
		typedef std::pair<int, bool> PairIB;
		typedef int iterator;
		typedef const int const_iterator;
		typedef NoneSubKey2 key_type;
		T insert(NoneSubKey2 &key){return make_pair(1,true);}
		T find(NoneSubKey2 &key){return NULL;}
		T end(){return NULL;}
		void erase(NoneSubKey2 &key){};
	};

}

template<typename MapType, typename MainKey, typename SubKey1=NoneSubKey1, typename SubKey2=NoneSubKey2>
class MultKeyMap  
{
	typedef std::map<MainKey, MapType> MainMap;
	typedef std::map<SubKey1, MapType*> SubMap1;
	typedef std::map<SubKey2, MapType*> SubMap2;
	typedef std::pair<typename MainMap::iterator, bool> PairIB;

public:
	bool insert(MapType &value)
	{
		PairIB pid = m_main.insert(make_pair(value.mainKey(), value));
		MapType *p_data = &(pid.first->second);
		if(pid.second)
		{
			if (m_sub1.insert(make_pair(value.subKey1(), p_data)).second)
			{
				if (m_sub2.insert(make_pair(value.subKey2(), p_data)).second)
				{
					return true;
				}
				else
				{
					m_sub1.erase(value.subKey1());
					m_main.erase(value.mainKey());
				}
			}
			else
			{
				m_main.erase(value.mainKey());
			}
		}
		return false;
	}

	//main
	MapType *find(typename MainMap::key_type &key)
	{
		typename MainMap::iterator it = m_main.find(key);
		if (it == m_main.end())
		{
			return NULL;
		}
		return &(it->second);
	}
	const MapType *find(typename MainMap::key_type &key) const
	{
		typename MainMap::const_iterator it = m_main.find(key);
		if (it == m_main.end())
		{
			return NULL;
		}
		return &(it->second);
	}
	
	//sub1
	MapType *find(typename SubMap1::key_type &key)
	{
		typename SubMap1::iterator it = m_sub1.find(key);
		if (it == m_sub1.end())
		{
			return NULL;
		}
		return (it->second);
	}

	const MapType *find(typename SubMap1::key_type &key) const
	{
		typename SubMap1::const_iterator it = m_sub1.find(key);
		if (it == m_sub1.end())
		{
			return NULL;
		}
		return (it->second);
	}
	//sub2
	MapType *find(typename SubMap2::key_type &key)
	{
		typename SubMap2::iterator it = m_sub2.find(key);
		if (it == m_sub2.end())
		{
			return NULL;
		}
		return (it->second);
	}

	const MapType *find(typename SubMap2::key_type &key) const
	{
		typename SubMap2::const_iterator it = m_sub2.find(key);
		if (it == m_sub2.end())
		{
			return NULL;
		}
		return (it->second);
	}
	
private:
	MainMap m_main;
	SubMap1 m_sub1;
	SubMap2 m_sub2;
};


