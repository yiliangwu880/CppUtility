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

enum class GameTaskType
{
	KILL_MONSTER = 1,
	LV = 2,
	GET_QUALITY_ITEM = 3,
	GET_ITEM = 4,
};

//任务参数逻辑
enum class TaskParaLogic
{
	EQUAL = 0,
	GREATER = 1,
	LESS = 2,
};

//配置所有任务类型
struct GameTaskTypeDetail
{
	//表达 Update参数和任务配置参数的关系。全部Update参数(排除最后一个)符合配置参数的关系，才算匹配该配置任务。
	//最后一个参数 表达 task_state.num(进度)  和 配置参数para的关系, 符合条件表示完成。
	std::vector<TaskParaLogic> vec_para_logic; 
	// true表示从Update传入的最后一个参数是绝对值，否则表示累加值.
	bool is_last_para_absolute; 
};
//获取配置单件
class GameTaskTypeMgr
{
public:
	static const GameTaskTypeMgr &Instance()
	{
		static GameTaskTypeMgr d;
		return d;
	}
	const std::map<GameTaskType, GameTaskTypeDetail> &GetCfg() const { return m_cfg; }
private:
	GameTaskTypeMgr()
	{
		m_cfg[GameTaskType::KILL_MONSTER] = 
		{
			{ TaskParaLogic::EQUAL, TaskParaLogic::GREATER },		//杀id怪， >= para2 
			//true, //不填，缺省值为is_last_para_absolute = false
		};
		m_cfg[GameTaskType::LV] =
		{
			{ TaskParaLogic::GREATER },		//等级 >= para1
			true,
		};
		m_cfg[GameTaskType::GET_QUALITY_ITEM] =
		{
			{ TaskParaLogic::EQUAL, TaskParaLogic::GREATER, TaskParaLogic::GREATER },		//获取id物品， >= 品质， >= para2
		};
		m_cfg[GameTaskType::GET_ITEM] =
		{
			{ TaskParaLogic::EQUAL, TaskParaLogic::GREATER },		//获取id物品， >= para2
		};
	}

private:
	std::map<GameTaskType, GameTaskTypeDetail> m_cfg;
};