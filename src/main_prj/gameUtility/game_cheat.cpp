/*
brief:ÓÎÏ·×÷±×Âë

*/



#include "game_cheat.h"
#include "../utility/string_tool.h"
#include "../utility/misc.h"

#define REG_CHEAT_FUN(fun)\
	MAP_REG_NAME_DEFINE(StrMapCheatFun, s_##fun, #fun, GameCheat::fun) 

using namespace std;

std::string GameCheat::executeCheatStr( GamePlayer *pPlayer, const char *pChar )
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
	ExecuteFun p_fun = it->second;
	return (*p_fun)(pPlayer, vec_str);
}

REG_CHEAT_FUN(doHandle1);
std::string GameCheat::doHandle1( GamePlayer *pPlayer, const VecStr &vecStr )
{
	string s;
	CONST_FOR_IT(VecStr, vecStr)
	{
		s+=*it;
	}
	return s;
}
