


#include "stdafx.h"
#include "../game_utility/game_cheat.h"


using namespace std;
using namespace wyl;

namespace
{
	void test1()
	{
		for (auto &v : StrMapCheatFun::obj())
		{
			string key = v.first;
		//	UNIT_INFO("key=%s", key.c_str());
		}

		string s = GameCheat::DoCmd(NULL, "doHandle1 abc,:ccd 123,321");
		//UNIT_INFO("para list: %s", s.c_str());
		UNIT_ASSERT(s == " abc ccd 123 321");
		s = GameCheat::DoCmd("doHandle1 abc,:ccd 123,321");
		UNIT_ASSERT(s == " abc ccd 123 321");

		s = GameCheat::DoCmd("MyClass_doHandle1 abc,:ccd 123,321");
		//UNIT_INFO("result=%s", s.c_str());
		UNIT_ASSERT(s == "static");


	}


	std::string doHandle1(GamePlayer *pPlayer, const VecStr &vecStr)
	{
		string s;
		CONST_FOR_IT(VecStr, vecStr)
		{
			s += " ";
			s += *it;
		}
		return s;
	}
	REG_PLAYER_DO_CMD(doHandle1);

	std::string doHandle1(const VecStr &vecStr)
	{
		string s;
		CONST_FOR_IT(VecStr, vecStr)
		{
			s += " ";
			s += *it;
		}
		return s;
	}
	REG_DO_CMD(doHandle1);

	class MyClass
	{
	public:
		static std::string doHandle1(const VecStr &vecStr)
		{
			UNIT_INFO("run doHandle1");
			return "static";
		}
	};
	REG_DO_CMD_EX(MyClass_doHandle1, MyClass::doHandle1)

}//end namespace

UNITTEST( test_game_cheat)
{

	test1();
	UNIT_INFO("done");
}