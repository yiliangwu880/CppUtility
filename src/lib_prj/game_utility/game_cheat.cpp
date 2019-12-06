/*
brief:游戏作弊码

*/



#include "game_cheat.h"
#include "../utility/string_tool.h"
#include "../utility/misc.h"


using namespace std;

std::string GameCheat::DoCmd( GamePlayer *pPlayer, const char *pChar )
{
	VecStr vec_str;
	VecStr vec_split;
	vec_split.push_back(";");
	vec_split.push_back(",");
	vec_split.push_back(".");
	vec_split.push_back(" ");
	vec_split.push_back(":");
	StringTool::split(pChar, vec_split, vec_str );
	if (vec_str.empty())
	{
		return "no string";
	}
	StrMapCheatFun::const_iterator it = StrMapCheatFun::obj().find(vec_str.front());
	if (it==StrMapCheatFun::obj().end())
	{
		return "can't find handle function";
	}
	vec_str.erase(vec_str.begin());
	DoPlayerCmdFun p_fun = it->second;
	return (*p_fun)(pPlayer, vec_str);
}

std::string GameCheat::DoCmd(const char *pChar)
{
	VecStr vec_str;
	VecStr vec_split;
	vec_split.push_back(";");
	vec_split.push_back(",");
	vec_split.push_back(".");
	vec_split.push_back(" ");
	vec_split.push_back(":");
	StringTool::split(pChar, vec_split, vec_str);
	if (vec_str.empty())
	{
		return "no string";
	}
	auto it = StrMapCheatFun_DoCmdFun::obj().find(vec_str.front());
	if (it == StrMapCheatFun_DoCmdFun::obj().end())
	{
		return "can't find handle function";
	}
	vec_str.erase(vec_str.begin());
	DoCmdFun p_fun = it->second;
	return (*p_fun)(vec_str);
}

