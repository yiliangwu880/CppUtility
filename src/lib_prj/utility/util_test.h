/*
	 �﷨��ࣺ
	 {
	 }
	 ��־���
	 �����쳣����

excamples:

UNITTEST(t1)
{
	UINT_ASSERT(1 == 2);
}

int main()
{
UnitTestMgr::obj().Start();
}

*/

#pragma once
#include <vector>

#define UNITTEST(Name)                                                   \
   class Test##Name : public IUnitTest                                            \
   {                                                                                     \
   public:                                                                               \
      Test##Name(){} \
   private:                                                                              \
      virtual void Run();                                                      \
   };                                         \
    static Test##Name  test##Name##Instance;                                   \
   void Test##Name::Run()




class IUnitTest
{
public:
	IUnitTest();
	virtual void Run() = 0;
private:

};

class UnitTestMgr
{
public:
	static UnitTestMgr &obj()
	{
		static UnitTestMgr d;
		return d;
	}
	void Start();
	void Reg(IUnitTest *p);
	void Log(const char *log);

private:
	UnitTestMgr() {};
private:
	std::vector<IUnitTest*> m_vecUnit;
};
#define TEST_LOG(x, ...)  //DebugLog::GetDefaultLog().printf(LOG_LV_INFO, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);


#define UINT_ASSERT(expression) do{                                                       \
            (expression)||                                                              \
            (TEST_LOG(#expression,) \
        }while(0)
