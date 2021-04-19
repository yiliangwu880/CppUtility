/*
brief: use example and test code
*/
#include "stdafx.h"
#include "../lib_prj/game_utility/Astar/astar.h"
#include "../lib_prj/game_utility/Astar/blockallocator.h"
#include "str_util.h"

using namespace std;
using namespace su;

UNITTEST(AStar)
{
	char maps[10][10] =
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

	// ËÑË÷²ÎÊý
	AStar::Params param;
	param.width = 10;
	param.height = 10;
	param.corner = false;
	param.start = AStar::Vec2(0, 0);
	param.end = AStar::Vec2(9, 9);
	param.can_pass = [&](const AStar::Vec2 &pos)->bool
	{
		return maps[pos.y][pos.x] == 0;
	};

	{
		std::string s;
		s += "\n";
		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				s += StrNum::NumToStr(maps[x][y]);
				s += " ";
			}
			s += "\n";
		}
		UNIT_INFO("%s\n\n", s.c_str());
	}

	// Ö´ÐÐËÑË÷
	BlockAllocator allocator;
	AStar algorithm(&allocator);
	auto path = algorithm.find(param);


	{
		for (const AStar::Vec2 &v : path)
		{
			UNIT_ASSERT(v.x < 10);
			UNIT_ASSERT(v.y < 10);
			UNIT_ASSERT(maps[v.y][v.x] == 0);
			maps[v.y][v.x] = 8;
		}
		std::string s;
		s += "\n";
		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				s += StrNum::NumToStr(maps[x][y]);
				s += " ";
			}
			s += "\n";
		}
		UNIT_INFO("%s", s.c_str());
	}
}


