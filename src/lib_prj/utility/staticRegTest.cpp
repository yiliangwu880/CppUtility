/*
brief: ¾²Ì¬×¢²áÊ¹ÓÃÀý×Ó
*/


#include "staticReg.h"
#include "misc.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;


	REG_SET_NAME_DECLARE(MyName, int)
	SET_REG_DEFINE(MyName, 0);
	SET_REG_DEFINE(MyName, 1);
	SET_REG_DEFINE(MyName, 2);

	REG_SET_NAME_DECLARE(MyName2, int)
	SET_REG_DEFINE(MyName2, 20);
	SET_REG_DEFINE(MyName2, 21);
	SET_REG_DEFINE(MyName2, 22);

	REG_MAP_NAME_DECLARE(MapName2, string, int)
	MAP_REG_NAME_DEFINE(MapName2, g_a, "a", 1);
	MAP_REG_NAME_DEFINE(MapName2, g_b, "abc", 2);
	MAP_REG_NAME_DEFINE(MapName2, g_b2, "accd", 2);
	MAP_REG_DEFINE(MapName2, bda, 2);
	MAP_REG_DEFINE(MapName2, ca, 2);



void testStaticReg()
{


	CONST_FOR_IT(MyName, MyName::obj())
	{
		cout << *it << endl;
	}

	CONST_FOR_IT(MyName2, MyName2::obj())
	{
		cout << *it << endl;
	}

	CONST_FOR_IT(MapName2, MapName2::obj())
	{
		cout << it->first << ":" << it->second << endl;
	}

}