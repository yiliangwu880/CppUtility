
#include "../lib_prj/utility/MulMap.h"
#include <unordered_map>

using namespace std;

struct Player
{
	char aar[1024 * 10];
	int i;
	int mainKey;
	string subkey;

};

template<>
struct MulMapTraits<Player>
{
	friend struct Player;
	static string GetSubKey(const Player &player)
	{
		return player.subkey;
	}
	static void SetKey(Player &player, int mainKey, string subKey)
	{
		player.mainKey = mainKey;
		player.subkey = subKey;
	}
};

namespace
{
	using MM = MulMap<int, string, Player>;
}


void fff1()
{
	MM mm;
	{//combine tset
		Player *p = mm.emplace(1, "abc", Player());
		UNIT_ASSERT(p);
		p->i = 5;
		auto p2 = mm.Find(1);
		UNIT_ASSERT(p2 == p);
		auto p3 = mm.SubFind("abc");
		UNIT_ASSERT(p3 == p);
		UNIT_ASSERT(!mm.Find(0));
		UNIT_ASSERT(!mm.Find(2));
		UNIT_ASSERT(!mm.SubFind(""));
		UNIT_ASSERT(!mm.SubFind("ab1"));
		mm.erase(1);
		UNIT_ASSERT(!mm.Find(1));
		UNIT_ASSERT(!mm.SubFind("abc"));
	}
	{
		{
			Player *p = mm.emplace(1, string(), Player());
			UNIT_ASSERT(p);
		}
		{
			Player *p = mm.emplace(2, string(), Player());
			UNIT_ASSERT(!p);
		}
		{
			Player *p = mm.emplace(2, string("2"), Player());
			UNIT_ASSERT(p);
		}
		{
			Player *p = mm.emplace(3, string("2"), Player());
			UNIT_ASSERT(!p);
		}

		UNIT_ASSERT(!mm.Find(0));

		UNIT_ASSERT(mm.Find(1));
		UNIT_ASSERT(mm.SubFind(""));

		UNIT_ASSERT(mm.Find(2));
		UNIT_ASSERT(mm.SubFind("2"));

		UNIT_ASSERT(!mm.Find(3));
		UNIT_ASSERT(!mm.SubFind("3"));
	}

}

UNITTEST(MulMap)
{
	return;
	fff1();
	MM mm;
	srand((int)time(NULL));
	for (; ; )
	{
		int r = rand();
		r = r % 4;
		int key = rand();
		key = key % 3000;

		string s;
		s.append(1, (char)rand());
		s.append(1, (char)rand());
		s.append(1, (char)rand());
		s.append(1, (char)rand());
		switch (r)
		{
		default:
			UNIT_ASSERT(false);
			break;
		case 0:
		{
			mm.emplace(key, s, Player());
		}
		break;
		case 1:
			mm.Find(key);
			break;
		case 2:
		{
			mm.SubFind(s);
		}
		break;
		case 3:
			mm.erase(key);
			UNIT_INFO("%d", mm.size());
			break;
		}

	}
	UNIT_INFO("run t1");
}


