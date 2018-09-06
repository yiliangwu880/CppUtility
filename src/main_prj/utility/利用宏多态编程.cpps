
//用整型映射函数
//////////////////////////////////////////////////////////////////////////
#define BASE_A \
	IMPLEMENT_BASE_A(V1,f1)	\
	IMPLEMENT_BASE_A(V2,f2) \
	IMPLEMENT_BASE_A(V3,f3) \


enum FunKey
{
#define IMPLEMENT_BASE_A(key,fun)	 key, 
	BASE_A
#undef  IMPLEMENT_BASE_A
	FUN_TABLE_SIZE,
};


class A 
{
public:
	typedef void (A::*PFun)();

	int i;
	void callMemberFun(FunKey key)
	{
		(this->*funTable[key])();
	}
	void f1()
	{
		Info("f1");
	}
	void f2()
	{
		Info("f2");
	}
	void f3()
	{
		Info("f3");
	}
	static PFun funTable[FUN_TABLE_SIZE];
};

A::PFun A::funTable[FUN_TABLE_SIZE]=
{
#define IMPLEMENT_BASE_A(key,fun)	&A::fun,
	BASE_A
#undef  IMPLEMENT_BASE_A
};


int _tmain(int argc, _TCHAR* argv[])
{
	A a;


	a.callMemberFun(V1);

	while(1);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
