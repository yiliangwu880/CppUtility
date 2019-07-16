


#include "stdafx.h"
#include "../gameUtility/game_cheat.h"


using namespace std;
using namespace wyl;

namespace
{
	void test1()
	{
		string s = GameCheat::executeCheatStr(NULL, "doHandle1 abc,:ccd 123,321");
		L_DEBUG("para list: %s", s.c_str());
		assert(true);
		
	}
}//end namespace

void test_game_cheat()
{

	test1();
	Info("done");
}