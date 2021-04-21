/*
brief: use example and test code
*/
#include <functional>
#include <limits>
#include <queue>
#include "../lib_prj/game_utility/Astar/astar.h"

using namespace std;
using namespace PathFind;

void PrintMap(char maps[10][10], const std::vector<Pos> &path)
{
	for (const Pos &v : path)
	{
		UNIT_ASSERT(v.x < 10);
		UNIT_ASSERT(v.y < 10);
		UNIT_ASSERT(maps[v.x][v.y] == 0);
		maps[v.x][v.y] = 8;
	}
	std::string s;
	s += "\n";
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			s += std::to_string(maps[x][y]);
			s += " ";
		}
		s += "\n";
	}
	UNIT_INFO("%s", s.c_str());
}
//使用例子
UNITTEST(AStar_example)
{
	return;
	char maps[10][10] =
	{
		//竖是 x坐标，横y坐标
		{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
		{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
		{ 0, 1, 0, 1, 1, 1, 1, 1, 0, 1 },
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 0, 1, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
	};


	Pos start(0, 0);
	Pos end(9, 9);

	IsBlockFun checkBlock = [&](const Pos &beforepos, const Pos &pos)->bool
	{
		if (pos.y >= 10 || pos.x >= 10)
		{
			return true;
		}
		return maps[pos.x][pos.y] != 0;
	};

	{
		std::string s;
		s += "\n";
		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				s += std::to_string(maps[x][y]);
				s += " ";
			}
			s += "\n";
		}
		UNIT_INFO("%s\n\n", s.c_str());
	}

	std::vector<Pos> path;
	auto ret = AStar::Ins().FindPath(start, end, path, checkBlock);
	UNIT_ASSERT(ret);
	UNIT_ASSERT(AStar::Ins().LastResult() == AStar::SUCCESS);

	PrintMap(maps, path);
}

namespace
{
	char g_maps[10][10] =
	{
		{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
		{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
		{ 0, 1, 0, 1, 1, 1, 1, 1, 0, 1 },
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 0, 1, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
	};
}

namespace
{
	IsBlockFun checkBlock = [&](const Pos &beforepos, const Pos &pos)->bool
	{
		if (pos.y >= 10 || pos.x >= 10)
		{
			return true;
		}
		return g_maps[pos.x][pos.y] != 0;
	};
}
//测试代码覆盖
UNITTEST(AStar_code_cover)
{
	{//目的地阻挡
		Pos start(0, 0);
		Pos end(1, 3);
		UNIT_ASSERT(g_maps[end.x][end.y] == 1);
		std::vector<Pos> path;
		auto ret = AStar::Ins().FindPath(start, end, path, checkBlock);
		UNIT_ASSERT(!ret);
		UNIT_ASSERT(AStar::Ins().LastResult() == AStar::UNREACHABLE);
	}
	{//目的地过远
		IsBlockFun checkBlock = [&](const Pos &beforepos, const Pos &pos)->bool
		{
			if (pos.y == 41 || pos.y == 41)//目的地不阻挡
			{
				return false;
			}
			if (pos.y >= 10 || pos.x >= 10)
			{
				return true;
			}
			return g_maps[pos.x][pos.y] != 0;
		};
		Pos start(0, 0);
		Pos end(41, 41);
		std::vector<Pos> path;
		auto ret = AStar::Ins().FindPath(start, end, path, checkBlock);
		UNIT_ASSERT(!ret);
		UNIT_ASSERT(AStar::Ins().LastResult() == AStar::TOO_FAR);
	}
	{//查找节点过多
		char maps[100][100] =
		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
			{ 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 0, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
			{ 1, 1, 0, 0, 1, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
		};
		AStar::Ins().SetMaxRadius(100);//10000点

		{
			IsBlockFun checkBlock1 = [&](const Pos &beforepos, const Pos &pos)->bool
			{
				if (pos.y >= 10 || pos.x >= 10)
				{
					return false;
				}
				return maps[pos.x][pos.y] != 0;
			};

			Pos start(0, 9);
			Pos end(3, 2); //围住的点
			std::vector<Pos> path;
			auto ret = AStar::Ins().FindPath(start, end, path, checkBlock1);
			UNIT_ASSERT(!ret);
			UNIT_ASSERT(AStar::Ins().LastResult() == AStar::TOO_MUCH_NODE);
		}

		{
			//遍历完可通过的节点，无法到达目的地
			IsBlockFun checkBlock2 = [&](const Pos &beforepos, const Pos &pos)->bool
			{
				if (pos.y >= 10 || pos.x >= 10)
				{
					return false;
				}
				return maps[pos.x][pos.y] != 0;
			};

			Pos start(0, 0);
			Pos end(2, 3); //围住的点
			std::vector<Pos> path;
			auto ret = AStar::Ins().FindPath(start, end, path, checkBlock2);
			UNIT_ASSERT(!ret);
			UNIT_ASSERT(AStar::Ins().LastResult() == AStar::UNREACHABLE);
		}
	}
	{//openlist 父节点改变
		char maps[10][10] =
		{
			{1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
			{1, 1, 1, 1, 1, 1, 0, 0, 1, 0},
			{1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
			{1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
			{1, 1, 0, 1, 0, 1, 0, 0, 1, 0},
			{1, 0, 1, 1, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
			{0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
			{0, 0, 1, 1, 1, 0, 0, 1, 0, 0},
			{0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
		};

		IsBlockFun checkBlock1 = [&](const Pos &beforepos, const Pos &pos)->bool
		{
			if (pos.y >= 10 || pos.x >= 10)
			{
				return true;
			}
			return maps[pos.x][pos.y] == 1;
		};

		Pos start(4, 4);
		Pos end(8, 1);
		std::vector<Pos> path;
		auto ret = AStar::Ins().FindPath(start, end, path, checkBlock1);
		UNIT_ASSERT(ret);
		UNIT_ASSERT(AStar::Ins().LastResult() == AStar::SUCCESS);
		//PrintMap(maps, path);
	}

	{//src 非法
		char maps[10][10] =
		{
			{1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
			{1, 1, 1, 1, 1, 1, 0, 0, 1, 0},
			{1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
			{1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
			{1, 1, 0, 1, 0, 1, 0, 0, 1, 0},
			{1, 0, 1, 1, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
			{0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
			{0, 0, 1, 1, 1, 0, 0, 1, 0, 0},
			{0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
		};

		IsBlockFun checkBlock1 = [&](const Pos &beforepos, const Pos &pos)->bool
		{
			if (pos.y >= 10 || pos.x >= 10)
			{
				return true;
			}
			return maps[pos.x][pos.y] == 1;
		};

		Pos start(4000, 4);
		Pos end(8, 1);
		std::vector<Pos> path;
		auto ret = AStar::Ins().FindPath(start, end, path, checkBlock1);
		UNIT_ASSERT(!ret);
	}
}
namespace
{
	//另一种用法,这种可能更好，待验证
	template<typename T, std::size_t N>
	constexpr std::size_t ArrayLen(T(&)[N])
	{ // constexpr
		return N; // and
	}
}

namespace
{
	char random_map[1000][1000];

	uint16_t GetEnd(uint16_t x)
	{
		uint16_t t;
		if (::rand() % 2)
		{
			 t =x + (uint16_t)(::rand() % 40);
		}
		else
		{
			t = x - (uint16_t)(::rand() % 40);
		}
		if (t >= ArrayLen(random_map))
		{
			t = ArrayLen(random_map) - 1;
		}
		return t;
	}
}
UNITTEST(AStar_random_test)
{
	size_t pathNum = 0;
	srand((unsigned int)time(0));
	for (int i = 0; i < 100 ; i++)
	{
		size_t maxLen = ArrayLen(random_map);
		for (size_t x = 0; x < maxLen; x++)
		{
			for (size_t y = 0; y < maxLen; y++)
			{
				if (::rand() % 3 == 0)
				{
					random_map[x][y] = 1;
				}
				else
				{
					random_map[x][y] = 0;
				}
			}
		}

		IsBlockFun checkBlock1 = [&](const Pos &beforepos, const Pos &pos)->bool
		{
			if (pos.x >= maxLen)return true;
			if (pos.y >= maxLen)return true;
			return random_map[pos.x][pos.y] == 1;
		};

		Pos start((uint16_t)(::rand() % maxLen), (uint16_t)(::rand() % maxLen));
		Pos end(GetEnd(start.x), GetEnd(start.y));
		std::vector<Pos> path;
		auto ret = AStar::Ins().FindPath(start, end, path, checkBlock1);
		if (ret)
		{
			UNIT_ASSERT(AStar::Ins().LastResult() == AStar::SUCCESS);	
			pathNum += path.size();
		}
		else
		{
			UNIT_ASSERT(AStar::Ins().LastResult() != AStar::SUCCESS);
		}

	}
	UNIT_INFO("pathNum =%ld", pathNum);
}

UNITTEST(AStar_test_effect)
{
	srand((unsigned int)time(0));
	size_t maxLen = ArrayLen(random_map);
	for (size_t x = 0; x < maxLen; x++)
	{
		for (size_t y = 0; y < maxLen; y++)
		{
			if (::rand() % 5 == 0)
			{
				random_map[x][y] = 1;
			}
			else
			{
				random_map[x][y] = 0;
			}
		}
	}

	time_t oldSec = time(0);
	size_t pathNum = 0;
	for (uint64_t i = 0; i < 1000 * 10; i++)
	{
		IsBlockFun checkBlock1 = [&](const Pos &beforepos, const Pos &pos)->bool
		{
			if (pos.x >= maxLen)return true;
			if (pos.y >= maxLen)return true;
			return random_map[pos.x][pos.y] == 1;
		};

		Pos start((uint16_t)(::rand() % maxLen), (uint16_t)(::rand() % maxLen));
		Pos end(GetEnd(start.x), GetEnd(start.y));

		std::vector<Pos> path;
		auto ret = AStar::Ins().FindPath(start, end, path, checkBlock1);
		if (ret)
		{
			UNIT_ASSERT(AStar::Ins().LastResult() == AStar::SUCCESS);
			pathNum += path.size();
		}
		else
		{
			UNIT_ASSERT(AStar::Ins().LastResult() != AStar::SUCCESS);
		}
	}
	time_t curSec = time(0);
	time_t cost = curSec - oldSec;
	UNIT_INFO("cost sec =%ld pathNum =%ld", cost, pathNum);
}