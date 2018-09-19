
#include "../utility/delay_para.h"

using namespace std;
using namespace wyl;

namespace
{
	class MyPara : public BaseDelayPara
	{
	public:
		MyPara(int p1, int p2)
		{
			a = p1; b = p2;
		}
		int a, b;
	};
	void test1()
	{
		LOG_DEBUG("test normal");
		{
			assert(DelayParaMgr::Instance().GetParaCnt() == 0);
			MyPara *p = DelayParaMgr::Instance().ConstructPara<MyPara>(1, 2);
			assert(p != NULL);


			DelayParaGuard<MyPara> guard(p);
			assert(guard.m_para != NULL);
			assert(guard.m_para->a == 1);
		}
		assert(DelayParaMgr::Instance().GetParaCnt() == 0);

		MyPara *p2 = 0;
		{

			MyPara *p = DelayParaMgr::Instance().ConstructPara<MyPara>(1, 2);
			assert(p != NULL);
			p2 = DelayParaMgr::Instance().ConstructPara<MyPara>(1, 2);
			assert(p2 != NULL);

			assert(DelayParaMgr::Instance().GetParaCnt() == 2);

			DelayParaGuard<MyPara> guard(p);
			assert(guard.m_para != NULL);
			assert(guard.m_para->a == 1);
		}
		assert(DelayParaMgr::Instance().GetParaCnt() == 1);
		{

			DelayParaGuard<MyPara> guard(p2);
			assert(guard.m_para != NULL);
			assert(guard.m_para->a == 1);
		}
		assert(DelayParaMgr::Instance().GetParaCnt() == 0);
	}

	void testTimer()
	{
		LOG_DEBUG("test time out");
		{
			assert(DelayParaMgr::Instance().GetParaCnt() == 0);
			MyPara *p = DelayParaMgr::Instance().ConstructPara<MyPara>(1, 2);
			assert(p != NULL);
			DelayParaMgr::Instance().SetTimeOutSec(4);
			sleep( 5);

			assert(DelayParaMgr::Instance().GetParaCnt() == 1);
			DelayParaMgr::Instance().OnTimer();
			assert(DelayParaMgr::Instance().GetParaCnt() == 0);

			DelayParaGuard<MyPara> guard(p);
			assert(guard.m_para == NULL);
		}
	}
}//end namespace

void testDelayPara()
{
	LOG_DEBUG("testDelayPara");
	test1();
	testTimer();
	LOG_DEBUG("done");
}