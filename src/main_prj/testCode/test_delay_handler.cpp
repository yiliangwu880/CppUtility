
#include "../gameUtility/delay_handler.h"
#include "../utility/cnt_typedef.h"
#include "../utility/easyTest.h"
#include "../utility/misc.h"

using namespace std;

namespace
{
	class MyTarget: public BaseDelayTarget
	{
	public:
		MyTarget()
		{
			m_opt1_cnt = m_opt2_cnt = 0;
		}

		uint32 m_opt1_cnt;	//操作 map 次数 
		uint32 m_opt2_cnt;	//操作 map 次数 
	};

	typedef void (*Fun)();

	class MyOpt1: public BaseDelayOpt
	{
	public:
		~MyOpt1()
		{
			if (NULL != m_end_fun)
			{
				m_end_fun();
			}
		}
		MyOpt1()
			:m_opt_data(0)
			,m_end_fun(NULL)
		{
		}
		MyOpt1(int i, char b)
			:m_opt_data(0)
			, m_end_fun(NULL)
		{
		}
		virtual void Handle(BaseDelayTarget &target)
		{
			MyTarget *my_target = dynamic_cast<MyTarget *>(&target);
			assert (NULL != my_target);
			my_target->m_opt1_cnt++;
		};
		uint32 m_opt_data;
		Fun m_end_fun;
	};

	class MyOpt2: public BaseDelayOpt
	{
	public:
		MyOpt2()
			:m_opt_data(0)
		{
		}
		virtual void Handle(BaseDelayTarget &target)
		{
			MyTarget *my_target = dynamic_cast<MyTarget *>(&target);
			assert (NULL != my_target);
			my_target->m_opt2_cnt++;
		};
		uint32 m_opt_data;
	};

	typedef std::map<uint64, MyTarget> Id2Target;
	Id2Target g_id_2_target;	//内存目标
	Id2Target g_db_id_2_target; //db目标
	VecUint64 g_req_db_id_ls;     //模拟请求DB的id

	class MyOptMgr: public BaseDelayOptMgr
	{
	public:
		virtual BaseDelayTarget *FindTarget(uint64 target_id)			//实现查找内存目标
		{
			Id2Target::iterator it = g_id_2_target.find(target_id);
			if (it == g_id_2_target.end())
			{
				return NULL;
			}
			return &(it->second);
		}

		virtual void MissTarget(uint64 target_id)			//实现，请求读档。  (缓存没有对象时会调用）
		{
			g_req_db_id_ls.push_back(target_id);
		}
	};

	MyOptMgr g_opt_mgr;

	uint32 g_opt1_end_cnt=0;
	void opt1_end()
	{
		g_opt1_end_cnt++;
	}
	void test1()
	{
		Info("-----------print error is ok---------------");
		MyOpt1 *opt1 = g_opt_mgr.CreateOpt<MyOpt1>();
		opt1->m_end_fun = opt1_end;
		uint32 t = g_opt1_end_cnt;
		MyOpt2 *opt2 =  g_opt_mgr.CreateOpt<MyOpt2>();
		assert(t+1==g_opt1_end_cnt); //opt1析构
		g_opt_mgr.AddOpt(1, opt1);
		g_opt_mgr.AddOpt(1, opt1);// opt1非法参数报错
		g_opt_mgr.AddOpt(1, NULL);// NULL非法参数报错
		g_opt_mgr.AddOpt(0, opt2);// opt2非法参数报错

		assert(1== g_opt_mgr.GetOptNum(1));


		WHILE_NUM(g_opt_mgr.MAX_REQ_NUM_PER_TARGET+1)
		{
			MyOpt1 *opt1 =  g_opt_mgr.CreateOpt<MyOpt1>();
			assert(t+1==g_opt1_end_cnt); 
			g_opt_mgr.AddOpt(2, opt1);//缓存溢出报错
		}
	

		uint64 last_id = 0;
		FOR_IT(VecUint64, g_req_db_id_ls)
		{
			uint64 id = *it;
			g_id_2_target[id]= MyTarget();
			MyTarget &target = g_id_2_target[id];
			g_opt_mgr.OptTarget(id, target);
			last_id = id;
		}
		g_req_db_id_ls.clear();

		Info("----------------end---------------");
		//递归调用
		g_id_2_target[1] = MyTarget();
		class MyOptRecursive : public BaseDelayOpt
		{
		public:
			MyOptRecursive()
				:m_opt_data(0)
			{
			}
			MyOptRecursive(int i, char b)
				:m_opt_data(0)
			{
				printf("use c++11\n");
			}
			virtual void Handle(BaseDelayTarget &target)
			{
				MyTarget *my_target = dynamic_cast<MyTarget *>(&target);
				assert(NULL != my_target);
				my_target->m_opt2_cnt++;

				MyOpt1 *opt1 = g_opt_mgr.CreateOpt<MyOpt1>();
				g_opt_mgr.AddOpt(1, opt1);
			};
			uint32 m_opt_data;
		};
		uint32 old_cnt = g_id_2_target[1].m_opt1_cnt;
		printf("below info must be:\"use c++11\"\n");
		BaseDelayOpt *opt = g_opt_mgr.CreateOpt<MyOptRecursive>(1,'a');
		g_opt_mgr.AddOpt(1, opt);
		assert(old_cnt + 1 == g_id_2_target[1].m_opt1_cnt);

	}
	void test2()
	{
		//延时请求
		g_id_2_target.clear();
		g_req_db_id_ls.clear();
		g_opt1_end_cnt = 0;
		BaseDelayOpt *opt = g_opt_mgr.CreateOpt<MyOpt1>();
		g_opt_mgr.AddOpt(1, opt);
		opt = g_opt_mgr.CreateOpt<MyOpt2>();
		g_opt_mgr.AddOpt(1, opt);
		assert(2== g_opt_mgr.GetOptNum(1));
		g_opt_mgr.DelOpt(1);
		assert(0 == g_opt_mgr.GetOptNum(1));

		opt = g_opt_mgr.CreateOpt<MyOpt1>();
		g_opt_mgr.AddOpt(1, opt);
		opt = g_opt_mgr.CreateOpt<MyOpt2>();
		g_opt_mgr.AddOpt(1, opt);
		assert(2== g_opt_mgr.GetOptNum(1));

		//读库模拟
		FOR_IT(VecUint64, g_req_db_id_ls)
		{
			uint64 id = *it;
			if (g_id_2_target.find(id) == g_id_2_target.end())
			{
				g_id_2_target[id]= MyTarget();
			}
			MyTarget &target = g_id_2_target[id];
			g_opt_mgr.OptTarget(id, target);
		}
		assert(0 == g_opt_mgr.GetOptNum(1));
		//uint32 cnt = g_id_2_target[1].m_opt1_cnt;
		assert(1 == g_id_2_target[1].m_opt1_cnt);
		assert(1 == g_id_2_target[1].m_opt2_cnt);

		//操作在内存对象
		opt = g_opt_mgr.CreateOpt<MyOpt1>();
		g_opt_mgr.AddOpt(1, opt);
		opt = g_opt_mgr.CreateOpt<MyOpt2>();
		g_opt_mgr.AddOpt(1, opt);
		assert(0== g_opt_mgr.GetOptNum(1));
		assert(2 == g_id_2_target[1].m_opt1_cnt);
		assert(2 == g_id_2_target[1].m_opt2_cnt);

		{
			opt = g_opt_mgr.CreateOpt<MyOpt1>();
			g_opt_mgr.AddOpt(2, opt);
			g_id_2_target[2]= MyTarget();
			//MyTarget &t = g_id_2_target[2];
			g_opt_mgr.OptTarget(2, g_id_2_target[2]);
			assert(1 == g_id_2_target[2].m_opt1_cnt);
		}
	}

	void test3()
	{
		WHILE_NUM(1000)
		{
			test2();
		}
	}
}

void test_delay_handler()
{
	test1();
	test2();
	test3();
	Info("test end");
}