

#include "stdafx.h"
#include "../utility/string_tool.h"


using namespace std;
namespace
{
	void test1()
	{
        string src;
        VecStr out;
        StringTool::split(src, " ", out);
        assert(out.empty());

        src = "a";
        StringTool::split(src, " ", out);
        assert(1 == out.size());
        assert("a"==out[0]);

        src = "a ";
        StringTool::split(src, " ", out);
        assert(1 == out.size());
        assert("a"==out[0]);

        src = " a ";
        StringTool::split(src, " ", out);
        assert(1 == out.size());
        assert("a"==out[0]);

        src = " a b";
        StringTool::split(src, " ", out);
        assert(2 == out.size());
        assert("a"==out[0]);
        assert("b"==out[1]);

        src = " a b";
        StringTool::split(src, " ", out, 1);
        assert(1 == out.size());
        assert("a"==out[0]);


	}
    void test2()
    {
        string s = "abckkabc";
        StringTool::replace(s, "", "1");
        StringTool::replace(s, 'a', '1');
        assert(s == "1bckk1bc");
        StringTool::replace(s, "1bc", "123");
        assert(s == "123kk123");
        StringTool::erase(s, "1bc");
        assert(s == "123kk123");
        StringTool::erase(s, "123");
        assert(s == "kk");
        StringTool::erase(s, "");
        assert(s == "kk");
        StringTool::erase(s, "1");
        assert(s == "kk");

    }
	void test3()
	{
		int i = 3;
		string s = StringTool::NumToStr(i);
		assert(s == "3");
		s = StringTool::NumToStr(0.30f);
		uint64 bb = 6000009898983433555;
		s = StringTool::NumToStr(bb);
		assert(s == "6000009898983433555");
	}

}//end namespace

void testStringTool()
{
	test1();
    test2();
	test3();
	Info("test string tool done");
}