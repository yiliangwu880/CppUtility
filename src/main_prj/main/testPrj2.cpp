


#include "stdafx.h"
#include <string>
#include <typeinfo>

using namespace std;

namespace
{

	class Base
	{
	public:
		virtual ~Base() = 0;
		int i;
	};

	class C : public Base
	{
		public:
		virtual ~C(){}
	};


}//namespace











void testPrj2()
{
    //time_t tNow =time(NULL);  
    //注意下面两行的区别  
//    struct tm* ptm = localtime(&tNow);  

    //int i = 3;

}

int t2f()
{
	return 3;
}



