/*
brief: use example and test code
*/
#include "stdafx.h"
#include "../game_utility/game_task.h"


using namespace std;
using namespace wyl;

namespace
{
	 struct  MyTaskCfg
	 {
		 uint32 id;
		 uint32 task_type;
		 uint32 para1, para2, para3, para4, para5;
	 };
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
		TaskMgr<MyTaskCfg> m_task_mgr;
		int m_finish_cnt, m_update_cnt;
	};
	class DeriveTask : public BaseTask<MyTaskCfg>
	{
	public:
		using  BaseTask_ = BaseTask<MyTaskCfg>;
		DeriveTask(const MyTaskCfg &cfg/*�����������ŵ�һ*/, Player &player, int64 num = 0)
			:BaseTask_(cfg, num)
			,m_player(player)
		{

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
	class DeriveTask2 : public BaseTask<MyTaskCfg>
	{
	public:
		using  BaseTask_ = BaseTask<MyTaskCfg>;
		DeriveTask2(const MyTaskCfg &cfg/*�����������ŵ�һ*/, Player &player, int64 num = 0)
			:BaseTask_(cfg, num)
			, m_player(player)
		{

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
		auto &mgr = player.m_task_mgr;

		{
			MyTaskCfg cfg = {
				1,//id
				(uint32)GameTaskType::LV,
				1 //para list
			};
			assert(0 == mgr.GetRegTaskNum());
			mgr.RegTask<DeriveTask>(cfg, player);
			assert(1 == mgr.GetRegTaskNum());

			mgr.Update(GameTaskType::LV, 0);
			assert(1 == mgr.GetRegTaskNum());
			mgr.Update(GameTaskType::LV, 1);
			assert(0 == mgr.GetRegTaskNum());
		}
		{//���Գ�ʼ�����Ⱦ������״̬
			MyTaskCfg cfg = {
				11,//id
				(uint32)GameTaskType::LV,
				1 //para list
			};
			mgr.RegTask<DeriveTask>(cfg, player, 1);

			mgr.Update(GameTaskType::LV, 0);
			assert(1 == mgr.GetRegTaskNum());
			mgr.Update(GameTaskType::LV, 1);
			assert(0 == mgr.GetRegTaskNum()); 
		}
		{//���Իص��Ƿ���Ч������ִ����ЧUpdate
			player.ClearCb();
			MyTaskCfg cfg = {
				2,//id
				(uint32)GameTaskType::GET_ITEM,
				1001, 2//para list
			};
			mgr.RegTask<DeriveTask>(cfg, player);
			mgr.Update(GameTaskType::GET_ITEM, 1002, 1);
			assert(1 == mgr.GetRegTaskNum());
			assert(0 == player.m_update_cnt);
			mgr.Update(GameTaskType::GET_ITEM, 1001, 1);
			assert(1 == player.m_update_cnt);
			assert(1 == mgr.GetRegTaskNum());
			assert(0 == player.m_finish_cnt);
			mgr.Update(GameTaskType::GET_ITEM, 1001, 1);
			assert(1 == player.m_finish_cnt);
			assert(0 == mgr.GetRegTaskNum());
		}
		{//��;��������
			player.ClearCb();
			MyTaskCfg cfg = {
				2,//id
				(uint32)GameTaskType::GET_ITEM,
				1001, 2//para list
			};
			mgr.RegTask<DeriveTask>(cfg, player);
			mgr.Update(GameTaskType::GET_ITEM, 1002, 1);
			assert(1 == mgr.GetRegTaskNum());
			mgr.Update(GameTaskType::GET_ITEM, 1001, 1);
			assert(1 == mgr.GetRegTaskNum());

			MyTaskCfg cfg2 = {
				22,//id
				(uint32)GameTaskType::GET_QUALITY_ITEM,
				1001, 2, 2//para list  --Ʒ�ʡ�2������>=2
			};
			mgr.RegTask<DeriveTask>(cfg2, player);
			assert(2 == mgr.GetRegTaskNum());
			mgr.Update(GameTaskType::GET_ITEM, 1001, 1);
			mgr.Update(GameTaskType::GET_QUALITY_ITEM, 1001, 2, 1);
			mgr.Update(GameTaskType::GET_QUALITY_ITEM, 1001, 1, 1); //��Ч
			mgr.Update(GameTaskType::GET_QUALITY_ITEM, 1002, 2, 1); //��Ч
			assert(1 == mgr.GetRegTaskNum()); //cfg�Ѿ����
			mgr.Update(GameTaskType::GET_QUALITY_ITEM, 1001, 2, 1);
			assert(0 == mgr.GetRegTaskNum());
		}
		{//unreg 
			MyTaskCfg cfg = {
				1,//id
				(uint32)GameTaskType::LV,
				1 //para list
			};
			MyTaskCfg cfg1 = {
				11,//id
				(uint32)GameTaskType::LV,
				1 //para list
			};
			assert(0 == mgr.GetRegTaskNum());
			mgr.RegTask<DeriveTask2>(cfg, player);
			mgr.RegTask<DeriveTask>(cfg1, player);
			assert(2 == mgr.GetRegTaskNum());
			mgr.UnRegTask(cfg);
			mgr.UnRegTask(cfg1);
			assert(0 == mgr.GetRegTaskNum());
		}

	}
}

void testGameTask()
{
	Info("testGameTask");
	TestRegNum();
	Info("testGameTask end");
}


