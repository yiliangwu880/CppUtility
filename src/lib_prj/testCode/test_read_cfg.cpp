/*
brief: use example and test code
*/
#include "stdafx.h"
#include "../utility/read_cfg.h"
#include <bitset>
#include "../utility/string_tool.h"

using namespace std;

namespace
{

    Config g_cfg;

	void test1()
	{
        g_cfg.init("test_cfg.txt");

        assert(string(g_cfg.GetStr("no"))==string(""));
        assert(string(g_cfg.GetStr("name1"))==string("string"));
        assert(string(g_cfg.GetStr("name2"))==string(" 3"));
        assert(string(g_cfg.GetStr("array"))==string(""));

        assert(g_cfg.GetInt("no")==0);
        assert(g_cfg.GetInt("name1")==0);
        assert(g_cfg.GetInt("name2")==3);
        assert(g_cfg.GetInt("array")==0);
        assert(g_cfg.GetUint64("longlong")==60020001000);


        VecStr vec_str;
        VecInt vec_int;
        g_cfg.GetStrArray("no", vec_str);
        assert(vec_str.empty());

        g_cfg.GetStrArray("array", vec_str);
        assert(vec_str.size()==3);
        assert(vec_str[2]=="3");

        g_cfg.GetIntArray("array", vec_int);
        assert(vec_int.size()==3);
        assert(vec_int[0]==1);
        assert(vec_int[1]==2);
        assert(vec_int[2]==3);

        g_cfg.GetStrArray("array2", vec_str);
        assert(vec_str.size()==3);
        assert(vec_str[2]=="b");

        const VecStr &repeat = g_cfg.GetRepeatName();
        assert(repeat.size()==2);
        assert(repeat[0]=="name2");
        assert(repeat[1]=="array");
        
        /*
        name1=string
        name2= 33
        name2 = 3
        longlong = 60020001000
        array:aa,2,b
        array:1,2,3
        array2:aa,2,b
            */

	}
} //end namespace

void testReadCfg()
{
	test1();

    Info("test read cfg done");
}


