

#include "stdafx.h"
#include "../utility/obj_pool.h"

using namespace std;
using namespace wyl;
namespace
{
    class MyObj
    {
    public:
        MyObj(){ buf[0]=0;}
        ~MyObj(){buf[0]=11;}

        string s;
        char buf[1024];
    };


    void testMemoryLeak1()
    {
        ObjPool<MyObj> pool(1);
        vector<MyObj*> vec;
        while (1)
        {
            int r = ::rand();
            if (r%10==0)
            {
                MyObj *p = pool.Create();
                p->buf[0] = 'b';
                p->s = "a";
                vec.push_back(p);
            }
            else
            {
                if (!vec.empty())
                {
                    pool.Destroy(vec.back());
                    vec.pop_back();
                }
            }

        }

    }
    void testMemoryLeak2()
    {
        ObjPool<MyObj> pool(2);

        MyObj *p = NULL ;

        p = pool.Create();
        assert(p->buf[0] == 0 );
        p->buf[0] = 1;
        pool.Destroy(p);
        assert(p->buf[0] == 11 );

        p = pool.Create();
        assert(p->buf[0] == 0 );
        p->buf[0] = 1;
        pool.Destroy(p);
        assert(p->buf[0] == 11 );
    }

	void TestIdIdxPool()
	{
		struct MyObj : public PoolIdIdxMgr<MyObj, 3>
		{
			int a;
			MyObj():a(1){};
			~MyObj(){a=2;}
		};

			assert(!MyObj::Free(443));

			assert(!MyObj::Free(0));

			MyObj *p1 = MyObj::Create();
			assert(p1->GetIdx()==0);
			assert(MyObj::Free(p1->Pid()));
			assert(!MyObj::Free(p1->Pid()));

			p1 = MyObj::Create();
			assert(p1->GetIdx()==0);
			MyObj *p2 = MyObj::Create();
			assert(p2->GetIdx()==1);

			assert(MyObj::GetObj(p2->Pid()) == p2);

			assert(MyObj::GetObj(p1->Pid()) != p2);
			assert(MyObj::GetObj(p1->Pid()) == p1);

			assert(MyObj::GetUseNum() == 2);

			MyObj *p3 = MyObj::Create();
			assert(NULL == MyObj::Create());
			assert(NULL == MyObj::Create());
			assert(MyObj::GetUseNum() == 3);

			assert(MyObj::Free(p1->Pid()));
			assert(MyObj::Free(p2->Pid()));
			assert(MyObj::GetUseNum() == 1);
			p1 = MyObj::Create();
			assert(MyObj::GetUseNum() == 2);
			p2 = MyObj::Create();
			assert(MyObj::GetUseNum() == 3);
			assert(MyObj::Free(p1->Pid()));
			assert(MyObj::Free(p2->Pid()));
			assert(MyObj::Free(p3->Pid()));
			assert(MyObj::GetUseNum() == 0);


			for (uint32 i=0; i<10; ++i)
			{
				assert(!MyObj::Free(443));

				MyObj *p1 = MyObj::Create();
				assert(MyObj::Free(p1->Pid()));
				assert(!MyObj::Free(p1->Pid()));

				p1 = MyObj::Create();
				MyObj *p2 = MyObj::Create();

				assert(MyObj::GetObj(p2->Pid()) == p2);

				assert(MyObj::GetObj(p1->Pid()) != p2);
				assert(MyObj::GetObj(p1->Pid()) == p1);

				assert(MyObj::GetUseNum() == 2);

				MyObj *p3 = MyObj::Create();
				assert(NULL == MyObj::Create());
				assert(NULL == MyObj::Create());
				assert(MyObj::GetUseNum() == 3);

				assert(MyObj::Free(p1->Pid()));
				assert(MyObj::Free(p2->Pid()));
				assert(MyObj::GetUseNum() == 1);
				p1 = MyObj::Create();
				assert(MyObj::GetUseNum() == 2);
				p2 = MyObj::Create();
				assert(MyObj::GetUseNum() == 3);
				assert(MyObj::Free(p1->Pid()));
				assert(MyObj::Free(p2->Pid()));
				assert(MyObj::Free(p3->Pid()));
				assert(MyObj::GetUseNum() == 0);

			}

	}
	void TestIdIdxPool2()
	{
		{//≤‚ ‘ππ‘Ï£¨ Œˆππ
			struct MyObj : public PoolIdIdxMgr<MyObj, 3>
			{
				int a;
				MyObj():a(1){};
				~MyObj(){a=2;}
			};

			MyObj *p = MyObj::Create();
			assert(p->a == 1);
			assert(MyObj::Free(p->Pid()));
			assert(p->a == 2);
		}

	}
}//end namespace

void testObjPool()
{
    srand(333);
	testMemoryLeak2();
	TestIdIdxPool();
	TestIdIdxPool2();
	Info("test obj pool done");
}