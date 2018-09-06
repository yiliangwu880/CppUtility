
#include "../boostCode/utility/misc.h"
#include <iostream>
#include "../utility/easyTest.h"
#include "../boostCode/utility/string_tool.h"


using namespace boost;
using namespace std;

namespace
{

	void test1()
	{

	}
	void test2()
	{
		string s = "a b,cd,43;";
		StrSplit ss(s);
		int i=0;
		BOOST_FOREACH( string sub_str, ss )
		{
			i++;
			if (i == 1){assert(sub_str == "a");}
			if (i == 2){assert(sub_str == "b");}
			if (i == 3){assert(sub_str == "cd");}
			if (i == 4){assert(sub_str == "43");}
		}


		std::string str = ";;Hello;|world||-123--456;yow;baz|";

		typedef boost::tokenizer<boost::char_separator<char> > 
			tokenizer2;
		boost::char_separator<char> sep("-;|");
		tokenizer2 tokens(str, sep);

		BOOST_FOREACH(const string &tok_iter, tokens) 
		{ 
			std::cout << "<" << tok_iter << "> ";
			std::cout << "\n";
		}
	}

}//namespace{



void testBoostUtiltity()
{
	Info("test boost utility");
	test1();
	test2();
	Info("done");
}