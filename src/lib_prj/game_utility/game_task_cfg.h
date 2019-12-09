/************************************************************************/
/*              
author:YiliangWu
brief:任务配置

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <map>
#include <vector>

enum class TaskType
{
	KILL_MONSTER     = 1,
	LV               = 2,
	GET_QUALITY_ITEM = 3,
	GET_ITEM         = 4,
};

//任务参数逻辑
enum class TaskParaLogic
{
	NONE,
	EQUAL   = 0,    //==
	GREATER = 1,    //>=
	LESS    = 2,    //<=
};

//配置所有任务类型
struct GameTaskTypeDetail
{
	std::vector<TaskParaLogic> vec_para_logic;	//表达 Update参数和任务配置参数的关系。 
	TaskParaLogic finish_logic;			//任务进度和 TaskCfg.paraX 最后一个有效参数（达成目标）的关系。
	bool is_last_para_absolute;			// true表示从Update传入的最后一个参数是绝对值，否则表示累加值.
};

struct  GameTaskLogicCfg
{
	//表达 Update参数和任务配置参数的关系。 
	//vec_para_logic[0] 表示update参数1 para1的关系，  vec_para_logic[1]表示。。。。
	std::vector<std::string> vec_para_logic;
	// task_state.num(任务进度) 和 TaskCfg.paraX 最后一个有效参数（达成目标）的关系。 
	std::string finish_logic;
	// true表示从Update传入的最后一个参数是绝对值，否则表示累加值.
	bool is_last_para_absolute;
};
using TaskType2Cfg = std::map<TaskType, GameTaskLogicCfg>;

//获取配置
class GameTaskTypeMgr
{
private:
	std::map<TaskType, GameTaskTypeDetail> m_cfg;
public:
	static const GameTaskTypeMgr &Instance()
	{
		static GameTaskTypeMgr d;
		return d;
	}
	const std::map<TaskType, GameTaskTypeDetail> &GetCfg() const { return m_cfg; }
private:
	static const TaskType2Cfg &GetTaskType2Cfg()
	{
		static TaskType2Cfg cfg=
		{
			{//para1==update::para1，任务进度 >= para2 ，Update传入的最后一个参数是累加值
				TaskType::KILL_MONSTER, 
				{{"==",}, ">=", false,}
			},
			{//任务进度 >= para1 ，Update传入的最后一个参数是累加值
				TaskType::LV, 
				{{}, ">=", true,}
			},					
			{//获取id物品， >= 品质， >= para2
				TaskType::GET_QUALITY_ITEM,
				{{"==", ">="}, ">=", false,}
			},
			{
				TaskType::GET_ITEM,
				{{"=="}, ">=", false,}
			},
		};
		return cfg;
	}
	TaskParaLogic Str2TaskParaLogic(const std::string &s)
	{
		if (s == "==")
		{
			return TaskParaLogic::EQUAL;
		}
		else if (s == ">=")
		{
			return TaskParaLogic::GREATER;
		}
		else if (s == "<=")
		{
			return TaskParaLogic::LESS;
		}
		else
		{
			L_ERROR("unknow logic type %s", s.c_str());
			return TaskParaLogic::NONE;
		}
	}
	GameTaskTypeMgr()
	{
		//字符串配置转换 配置，加快效率
		const TaskType2Cfg &str_cfg= GameTaskTypeMgr::GetTaskType2Cfg();
		for (const auto &v: str_cfg)
		{
			const GameTaskLogicCfg &cfg = v.second;
			GameTaskTypeDetail &m_cfg_v = m_cfg[v.first];
			for (const std::string &s : cfg.vec_para_logic)
			{
				m_cfg_v.vec_para_logic.push_back(Str2TaskParaLogic(s));
			}
			m_cfg_v.finish_logic = Str2TaskParaLogic(cfg.finish_logic);
			m_cfg_v.is_last_para_absolute = cfg.is_last_para_absolute;
		}
	}

};
