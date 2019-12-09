/*
brief: use example and test code
*/
#include "stdafx.h"
#include "../game_utility/game_task.h"


using namespace std;
using namespace wyl;


namespace
{

	class Player
	{
	public:
		Player(int uin)
			:m_uin(uin)
			, m_finish_cnt(0)
			, m_update_cnt(0)
		{
		}
		void ClearCb()
		{
			m_finish_cnt = 0;
			m_update_cnt = 0;
		}
		int m_uin;
		TaskMgr m_task_mgr;
		int m_finish_cnt, m_update_cnt;
	};
	class DeriveTask : public BaseTask
	{
	public:
		using  BaseTask_ = BaseTask;
		DeriveTask(const TaskCfg &cfg/*这个参数必须放第一*/, Player &player, int64 num = 0)
			:BaseTask_(cfg, num)
			,m_player(player)
		{

		}
		virtual void OnFinish() override
		{
			m_player.m_finish_cnt++;
		};//可以做存档操作，比如存task_id, num, is_finish
		virtual void OnUpdate()override
		{
			m_player.m_update_cnt++;
		};//可以做存档操作，比如存task_id, num
		Player &m_player;
	};
	class DeriveTask2 : public BaseTask
	{
	public:
		using  BaseTask_ = BaseTask;
		DeriveTask2(const TaskCfg &cfg/*这个参数必须放第一*/, Player &player, int64 num = 0)
			:BaseTask_(cfg, num)
			, m_player(player)
		{

		}
		virtual void OnFinish() override
		{
			m_player.m_finish_cnt++;
		};//可以做存档操作，比如存task_id, num, is_finish
		virtual void OnUpdate()override
		{
			m_player.m_update_cnt++;
		};//可以做存档操作，比如存task_id, num
		Player &m_player;
	};

	void TestRegNum()
	{
		Player player(5);
		auto &mgr = player.m_task_mgr;

		{
			TaskCfg cfg = {
				1,//id
				(uint32)TaskType::LV,
				1 //para list
			};
			UNIT_ASSERT(0 == mgr.GetRegTaskNum());
			mgr.RegTask<DeriveTask>(cfg, player);
			UNIT_ASSERT(1 == mgr.GetRegTaskNum());

			mgr.Update(TaskType::LV, 0);
			UNIT_ASSERT(1 == mgr.GetRegTaskNum());
			mgr.Update(TaskType::LV, 1);
			UNIT_ASSERT(0 == mgr.GetRegTaskNum());
		}
		{//测试初始化进度就是完成状态
			TaskCfg cfg = {
				11,//id
				(uint32)TaskType::LV,
				1 //para list
			};
			mgr.RegTask<DeriveTask>(cfg, player, 1);

			mgr.Update(TaskType::LV, 0);
			UNIT_ASSERT(1 == mgr.GetRegTaskNum());
			mgr.Update(TaskType::LV, 1);
			UNIT_ASSERT(0 == mgr.GetRegTaskNum()); 
		}
		{//测试回调是否有效，测试执行无效Update
			player.ClearCb();
			TaskCfg cfg = {
				2,//id
				(uint32)TaskType::GET_ITEM,
				1001, 2//para list
			};
			mgr.RegTask<DeriveTask>(cfg, player);
			mgr.Update(TaskType::GET_ITEM, 1002, 1);
			UNIT_ASSERT(1 == mgr.GetRegTaskNum());
			UNIT_ASSERT(0 == player.m_update_cnt);
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			UNIT_ASSERT(1 == player.m_update_cnt);
			UNIT_ASSERT(1 == mgr.GetRegTaskNum());
			UNIT_ASSERT(0 == player.m_finish_cnt);
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			UNIT_ASSERT(1 == player.m_finish_cnt);
			UNIT_ASSERT(0 == mgr.GetRegTaskNum());
		}
		{//中途加新任务
			player.ClearCb();
			TaskCfg cfg = {
				2,//id
				(uint32)TaskType::GET_ITEM,
				1001, 2//para list
			};
			mgr.RegTask<DeriveTask>(cfg, player);
			mgr.Update(TaskType::GET_ITEM, 1002, 1);
			UNIT_ASSERT(1 == mgr.GetRegTaskNum());
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			UNIT_ASSERT(1 == mgr.GetRegTaskNum());

			TaskCfg cfg2 = {
				22,//id
				(uint32)TaskType::GET_QUALITY_ITEM,
				1001, 2, 2//para list  --品质》2，数量>=2
			};
			mgr.RegTask<DeriveTask>(cfg2, player);
			UNIT_ASSERT(2 == mgr.GetRegTaskNum());
			mgr.Update(TaskType::GET_ITEM, 1001, 1);
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1001, 2, 1);
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1001, 1, 1); //无效
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1002, 2, 1); //无效
			UNIT_ASSERT(1 == mgr.GetRegTaskNum()); //cfg已经完成
			mgr.Update(TaskType::GET_QUALITY_ITEM, 1001, 2, 1);
			UNIT_ASSERT(0 == mgr.GetRegTaskNum());
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
			UNIT_ASSERT(0 == mgr.GetRegTaskNum());
			mgr.RegTask<DeriveTask2>(cfg, player);
			mgr.RegTask<DeriveTask>(cfg1, player);
			UNIT_ASSERT(2 == mgr.GetRegTaskNum());
			mgr.UnRegTask(cfg);
			mgr.UnRegTask(cfg1);
			UNIT_ASSERT(0 == mgr.GetRegTaskNum());
		}

	}
}

UNITTEST(testGameTask)
{
	UNIT_INFO("testGameTask");
	TestRegNum();
	UNIT_INFO("testGameTask end");
}


