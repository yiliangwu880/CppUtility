
#include "../lib_prj/game_utility/delay_handler.h"
#include "../lib_prj/utility/cnt_typedef.h"
#include "../lib_prj/utility/misc.h"
#include "../lib_prj/unit_test/unit_test.h"

using namespace std;
using namespace placeholders;

namespace
{
	class MyTarget
	{
	public:
		MyTarget()
		{
			m_opt1_cnt = m_opt2_cnt = 0;
		}

		uint32 m_opt1_cnt;	//���� map ���� 
		uint32 m_opt2_cnt;	//���� map ���� 
	};

	typedef std::map<uint64, MyTarget> Id2Target;
	Id2Target g_id_2_target;	//�ڴ�Ŀ��
	Id2Target g_db_id_2_target; //dbĿ��
	VecUint64 g_req_db_id_ls;     //ģ������DB��id

	class MyOptMgr: public DelayOptMgr<MyTarget, uint64>
	{
	public:
		virtual MyTarget *OnFindTarget(uint64 target_id)			//ʵ�ֲ����ڴ�Ŀ��
		{
			Id2Target::iterator it = g_id_2_target.find(target_id);
			if (it == g_id_2_target.end())
			{
				return NULL;
			}
			return &(it->second);
		}

		virtual void OnMissTarget(uint64 target_id)			//ʵ�֣����������  (����û�ж���ʱ����ã�
		{
			g_req_db_id_ls.push_back(target_id);
		}
	};

	MyOptMgr g_opt_mgr;

	void bindFun(MyTarget &my_target, uint32 add)
	{
		my_target.m_opt1_cnt += add;
	}
	void test1()
	{
		UNIT_INFO("-----------print error is ok---------------");
		auto  opt1 = [](MyTarget &obj){obj.m_opt1_cnt++;};	
		auto  opt2 = [](MyTarget &obj){obj.m_opt2_cnt++;};
		g_opt_mgr.AddOpt(1, opt1);
		g_opt_mgr.AddOpt(1, opt1);// opt1�Ƿ���������
		g_opt_mgr.AddOpt(1, NULL);// NULL�Ƿ���������
		g_opt_mgr.AddOpt(0, opt2);// opt2�Ƿ���������

		UNIT_ASSERT(2 == g_opt_mgr.GetOptNum(1));

		UNIT_INFO("-----------print error is ok---------------");
		WHILE_NUM(g_opt_mgr.GetMaxNum() + 1)
		{
			g_opt_mgr.AddOpt(2, opt1);//�����������
		}


		uint64 last_id = 0;
		FOR_IT(VecUint64, g_req_db_id_ls)
		{
			uint64 id = *it;
			g_id_2_target[id] = MyTarget();
			MyTarget &target = g_id_2_target[id];
			g_opt_mgr.OptTarget(id, target);
			last_id = id;
		}
		g_req_db_id_ls.clear();

		UNIT_INFO("----------------end---------------");

		//�ݹ����
		g_id_2_target[1] = MyTarget();

		auto opt = [](MyTarget &my_target)
		{
			my_target.m_opt2_cnt++;
			auto  opt1 = [](MyTarget &obj) {obj.m_opt1_cnt++; };
			g_opt_mgr.AddOpt(1, opt1);
		};
		uint32 old_cnt = g_id_2_target[1].m_opt1_cnt;
		g_opt_mgr.AddOpt(1, opt);
		UNIT_ASSERT(old_cnt + 1 == g_id_2_target[1].m_opt1_cnt);

		//std::bind
		{
			uint32 old_cnt = g_id_2_target[1].m_opt1_cnt;
			auto f = std::bind(bindFun, _1, 3);
			g_opt_mgr.AddOpt(1, f);
			UNIT_ASSERT(old_cnt + 3 == g_id_2_target[1].m_opt1_cnt);
		}
	}

	void test2()
	{
		//��ʱ����
		g_id_2_target.clear();
		g_req_db_id_ls.clear();
		auto  opt1 = [](MyTarget &obj) {obj.m_opt1_cnt++; };
		auto  opt2 = [](MyTarget &obj) {obj.m_opt2_cnt++; };

		g_opt_mgr.AddOpt(1, opt1);
		g_opt_mgr.AddOpt(1, opt2);
		UNIT_ASSERT(2== g_opt_mgr.GetOptNum(1));
		g_opt_mgr.DelOpt(1);
		UNIT_ASSERT(0 == g_opt_mgr.GetOptNum(1));

		g_opt_mgr.AddOpt(1, opt1);
		g_opt_mgr.AddOpt(1, opt2);
		UNIT_ASSERT(2== g_opt_mgr.GetOptNum(1));

		//����ģ��
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
		UNIT_ASSERT(0 == g_opt_mgr.GetOptNum(1));
		UNIT_ASSERT(1 == g_id_2_target[1].m_opt1_cnt);
		UNIT_ASSERT(1 == g_id_2_target[1].m_opt2_cnt);

		//�������ڴ����
		g_opt_mgr.AddOpt(1, opt1);
		g_opt_mgr.AddOpt(1, opt2);
		UNIT_ASSERT(0== g_opt_mgr.GetOptNum(1));
		UNIT_ASSERT(2 == g_id_2_target[1].m_opt1_cnt);
		UNIT_ASSERT(2 == g_id_2_target[1].m_opt2_cnt);

		{
			g_id_2_target[2] = MyTarget();
			UNIT_ASSERT(0 == g_id_2_target[2].m_opt1_cnt);
			g_opt_mgr.AddOpt(2, opt1);
			UNIT_ASSERT(1 == g_id_2_target[2].m_opt1_cnt);
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


UNITTEST(test_delay_handler)
{
	test1();
	test2();
	test3();
	UNIT_INFO("---test_delay_handler end---");

}