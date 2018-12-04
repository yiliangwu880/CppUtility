#include <iostream>
using namespace std;

放弃吧，参考更好实现 ScopeGuard

template<typename T>
class Defer{
	public:
	T *t;
	Defer(T *i)
	{
		t = i;
	}
	~Defer()
	{
		(*t)();
	}
};

#define DEFER(a)\
	do\
	{\
	auto func = a;\
	Defer<decltype(func)> d(&func);	\
	}while(0);

void t1()
{
	int a = 3;
	auto b = a;

	auto func = [&a](){cout << "run+" <<a<< endl;};
	Defer<decltype(func)> d(&func);	
	
	
	DEFER(([&b](){cout << "abc"<< b << endl;}));
}

int main()
{
	t1();

   cout << "Hello Worlda";
   return 0;
}
