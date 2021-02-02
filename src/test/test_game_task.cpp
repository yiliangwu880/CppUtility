/*
brief: use example and test code
*/
#include "stdafx.h"
#include "../lib_prj/game_utility/game_task.h"


using namespace std;


namespace
{
	struct MyTaskMgr : public TaskMgr
	{
		MyTaskMgr()
		{
		}
		~MyTaskMgr()
		{
		}
	};
	class Player
	{
	public:
		int m_uin;
		int m_finish_cnt, m_update_cnt, m_del_cnt;
		TaskMgr m_TaskMgr;

		Player(int uin)
			:m_uin(uin)
			, m_finish_cnt(0)
			, m_update_cnt(0)
			, m_del_cnt()
			, m_TaskMgr()
		{
		}
		void ClearCb()
		{
			m_finish_cnt = 0;
			m_update_cnt = 0;
		}
	};
	class DeriveTask : public BaseTask
	{
	public:
		DeriveTask(const TaskCfg &cfg, Player &player, int64 num = 0)
			:BaseTask(cfg, num)
			,m_player(player)
		{

		}
		virtual ~DeriveTask() override
		{
			m_player.m_del_cnt++;

		}
		virtual void OnFinish() override
		{
			m_player.m_finish_cnt++;
		};//�������浵�����������task_id, num, is_finish
		virtual void OnUpdate()override
		{
			m_player.m_update_cnt++;
		};//�������浵�����������task_id, num
		Player &m_player;
	};

	class DeriveTask2 : public BaseTask
	{
	public:
		DeriveTask2(const TaskCfg &cfg/*�����������ŵ�һ*/, Player &player, int64 num = 0)
			:BaseTask(cfg, num)
			, m_player(player)
		{

		}
		virtual ~DeriveTask2() override
		{

			m_player.m_del_cnt++;
		}
		virtual void OnFinish() override
		{
			m_player.m_finish_cnt++;
		};//�������浵�����������task_id, num, is_finish
		virtual void OnUpdate()override
		{
			m_player.m_update_cnt++;
		};//�������浵�����������task_id, num
		Player &m_player;
	};
	
	void TestRegNum()
	{
		Player player(5);
		auto &mgr = (player.m_TaskMgr);
		list<uint64> vecTask;
		list<uint64> vecTask2;//DeriveTask2
		{
			static TaskCfg cfg = {
				1,//id
				(uint32)TaskType::LV,
				1 //para list
			};
			UNIT_ASSERT(0 == mgr.GetTaskNum());
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg, player));
			UNIT_ASSERT(1 == mgr.GetTaskNum());



			mgr.Update(TaskType::LV, 0);
			UNIT_ASSERT(1 == mgr.GetTaskNum());
			mgr.Update(TaskType::LV, 1);
			UNIT_ASSERT(0 == mgr.GetTaskNum());

			UNIT_ASSERT(!mgr.IsFinish(cfg, 0));
			UNIT_ASSERT(mgr.IsFinish(cfg, 1));
		}
		

		{//������ͬTaskCfg����� BaseTask

			static TaskCfg cfg = {
				2,//id
				(uint32)TaskType::LV,
				1 //para list
			};
			vecTask.clear();
			UNIT_ASSERT(0 == mgr.GetTaskNum());
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg, player));
			UNIT_ASSERT(1 == mgr.GetTaskNum());
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg, player));
			UNIT_ASSERT(2 == mgr.GetTaskNum());


			mgr.Update(TaskType::LV, 0);

			UNIT_ASSERT(2 == mgr.GetTaskNum());
			mgr.Update(TaskType::LV, 1);
			UNIT_ASSERT(0 == mgr.GetTaskNum());
		}
		{//���Գ�ʼ�����Ⱦ������״̬
			static TaskCfg cfg = {
				11,//id
				(uint32)TaskType::LV,
				1 //para list
			};
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg, player, 1));

			mgr.Update(TaskType::LV, 0 ); 
			UNIT_ASSERT(1 == mgr.GetTaskNum());
			mgr.Update(TaskType::LV, 1);
			UNIT_ASSERT(0 == mgr.GetTaskNum()); 
		}
		{//���Իص��Ƿ���Ч������ִ����ЧUpdate
			player.ClearCb();
			static TaskCfg cfg = {
				2,//id
				(uint32)TaskType::GET_ITEM,
				1001, 2//para list
			};
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg, player));
			mgr.Update(TaskType::GET_ITEM, 1002, 1);
			UNIT_ASSERT(1 == mgr.GetTaskNum());
			UNIT_ASSERT(0 == player.m_update_cnt);
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			UNIT_ASSERT(1 == player.m_update_cnt);
			UNIT_ASSERT(1 == mgr.GetTaskNum());
			UNIT_ASSERT(0 == player.m_finish_cnt);
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			UNIT_ASSERT(1 == player.m_finish_cnt);
			UNIT_ASSERT(0 == mgr.GetTaskNum());


			UNIT_ASSERT(!mgr.IsFinish(cfg, 0));
			UNIT_ASSERT(!mgr.IsFinish(cfg, 1));
			UNIT_ASSERT(mgr.IsFinish(cfg, 2));
		}
		{//��;��������
			player.ClearCb();
			TaskCfg cfg = {
				2,//id
				(uint32)TaskType::GET_ITEM,
				1001, 2//para list
			};
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg, player));
			mgr.Update(TaskType::GET_ITEM, 1002, 1);
			UNIT_ASSERT(1 == mgr.GetTaskNum());
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			UNIT_ASSERT(1 == mgr.GetTaskNum());

			TaskCfg cfg2 = {
				22,//id
				(uint32)TaskType::GET_QUALITY_ITEM,
				1001, 2, 2//para list  --Ʒ�ʡ�2������>=2
			};
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg2, player));
			UNIT_ASSERT(2 == mgr.GetTaskNum());
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1001, 2, 1);
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1001, 1, 1); //��Ч
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1002, 2, 1); //��Ч
			UNIT_ASSERT(1 == mgr.GetTaskNum()); //cfg�Ѿ����
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1001, 2, 1);
			UNIT_ASSERT(0 == mgr.GetTaskNum());
		}
		{//unreg 
			TaskCfg cfg = {
				1,//id
				(uint32)TaskType::LV,
				1 //para list
			};
			TaskCfg cfg1 = {
				11,//id
				(uint32)TaskType::LV,
				1 //para list
			};
			UNIT_ASSERT(0 == mgr.GetTaskNum());
			vecTask2.push_back(mgr.CreateTask<DeriveTask2>(cfg, player));
			vecTask.push_back(mgr.CreateTask<DeriveTask>(cfg1, player));
			UNIT_ASSERT(2 == mgr.GetTaskNum());
			int del_cnt = player.m_del_cnt;
			mgr.DestoryTask(vecTask2.back());
			vecTask2.pop_back();
			UNIT_ASSERT(del_cnt + 1 == player.m_del_cnt);
			UNIT_ASSERT(1 == mgr.GetTaskNum());
			mgr.DestoryTask(vecTask.back());
			vecTask.pop_back();
			UNIT_ASSERT(del_cnt + 2 == player.m_del_cnt);
			UNIT_ASSERT(0 == mgr.GetTaskNum());
		}

	}

	void Testtmp()
	{
		Player player(5);
		auto &mgr = (player.m_TaskMgr);
		list<DeriveTask> vecTask;
		list<DeriveTask2> vecTask2;

		static TaskCfg cfg = {
			1,//id
			(uint32)TaskType::LV,
			1 //para list
		};
		DeriveTask t(cfg, player);

		DeriveTask t2(cfg, player);
		//t = std::move(t2);
	}

}

UNITTEST(testGameTask)
{
	UNIT_INFO("testGameTask");
	TestRegNum();
	UNIT_INFO("testGameTask end");
}


