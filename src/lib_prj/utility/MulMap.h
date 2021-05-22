#pragma once
#include <unordered_map>
#include <utility>

template<class mapped_type>
struct MulMapTraits {};

//template<>
//struct MulMapTraits<Player>
//{
//	friend struct Player;
//	static string GetSubKey(const Player &player)
//	{
//		return player.subkey;
//	}
//	static void SetKey(Player &player, int mainKey, string subKey)
//	{
//		player.mainKey = mainKey;
//		player.subkey = subKey;
//	}
//};

//双索引map. 
//注意：一但创建，两个key不能让用户修改。，修改就出错
template<class MainKey, class SubKey, class mapped_type>
class MulMap
{
	unordered_map<MainKey, mapped_type> main;
	unordered_map<SubKey, mapped_type*> sub;
public:
	template<class... _Valty>
	mapped_type *emplace(MainKey mainKey, SubKey subKey, _Valty&&... _Val)
	{
		auto it = main.find(mainKey);
		if (it != main.end())
		{
			return false;
		}
		auto subIt = sub.find(subKey);
		if (subIt != sub.end())
		{
			return false;
		}
		it = main.emplace(make_pair(mainKey, forward<_Valty>(_Val)...)).first;
		mapped_type *p = &(it->second);
		sub.emplace(make_pair(subKey, p));
		MulMapTraits<mapped_type>::SetKey(*p, mainKey, subKey);
		return p;
	}

	size_t size()
	{
		return main.size();
	}
	bool erase(MainKey mainKey)
	{
		auto it = main.find(mainKey);
		if (it == main.end())
		{
			return false;
		}
		mapped_type *p = &(it->second);
		SubKey subKey = MulMapTraits<mapped_type>::GetSubKey(*p);

		auto subIt = sub.find(subKey);
		if (subIt == sub.end())
		{
			UNIT_ERROR("subkey change!");
			return false;
		}
		if (p != (subIt->second))
		{
			UNIT_ERROR("subkey change!");
			return false;
		}

		main.erase(it);
		sub.erase(subIt);
		return true;
	}

	mapped_type *Find(MainKey mainKey)
	{
		auto it = main.find(mainKey);
		if (it == main.end())
		{
			return nullptr;
		}
		return &(it->second);
	}
	mapped_type *SubFind(SubKey subKey)
	{
		auto it = sub.find(subKey);
		if (it == sub.end())
		{
			return nullptr;
		}
		return it->second;
	}
};