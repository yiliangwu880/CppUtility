
#include "../utility/mult_key_map.h"


using namespace std;

namespace
{
	struct Data 
	{
		int mainKey(){return 0;};
		char subKey1(){return 0;};
		double subKey2(){return 0;};
	};
	struct DataDefault 
	{
		int mainKey(){return 0;};
		char subKey1(){return 0;};
		double subKey2(){return 0;};
	};
	void test1()
	{
		{
			MultKeyMap<Data, int, char, double> m;
			char a;
			m.find(a);
		}

		{
			MultKeyMap<DataDefault, int> m;
			int ia;
			m.find(ia);
		}

	}

}//namespace{



void test_mult_key_map()
{
	test1();
	Info("done");
}