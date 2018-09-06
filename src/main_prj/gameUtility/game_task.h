/************************************************************************/
/*
author:YiliangWu
brief:

任务功能
新任务需要做:
新任务类型，需要GameTaskTypeMgr构造函数里面加GameTaskTypeDetail配置
新任务， 策划配置按照 GameTaskTypeDetail 要求填值
注册任务事件调用： TaskMgr<TaskCfg>::RegTask
任务更新触发调用：[TaskMgr<TaskCfgTaskCfg>::Update(task_type, ...)]

使用例子：
	class DeriveTask: public BaseTask<TaskCfg>
	{
	public:
		DeriveTask(const MyTaskCfg &cfg 这个参数必须放第一 , Player &player, int64 num = 0);
		virtual void OnFinish() override{};//可以做存档操作，比如存task_id, num, is_finish
		virtual void OnUpdate()override{};//可以做存档操作，比如存task_id, num
	}

	//注册任务
	TaskMgr<TaskCfg> mgr;
	Player player;
	TaskCfg cfg; 
	...
	mgr.RegTask<DeriveTask>(cfg, player, [1]);
	...

	//当触发任务改变事件发生时调用
	obj:Update(target_type.GET_ITEM, 1002, 1)；

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <map>
#include "config.h"
#include <algorithm>
#include "game_task_cfg.h"
#include <stdarg.h>
#include "../utility/stlBoost.h"


//策划配置模板，根据不同项目改命名，参数名字要一样。
//struct  TaskCfg
//{
//	uint32 id;
//	uint32 task_type;
//	uint32 para1, para2, para3, para4, para5;
//};



//派生类的构造函数，第一个参数必须为 const TaskCfg &
template<class TaskCfg>
class BaseTask
{
public:
	//para num 任务初始化进度
	BaseTask(const TaskCfg &cfg, int64 num)
		:m_num(num)
		, m_cfg(cfg)
	{
	}

	virtual ~BaseTask(){};

	virtual void OnFinish() = 0;
	virtual void OnUpdate() = 0;

	const TaskCfg &GetCfg(){ return m_cfg; }
	void SetNum(int64 num){ m_num = num; }
	void AddNum(int64 add){ m_num += add; }
	const int64 GetNum(){ return m_num; }
protected:
	int64 m_num; //表示进度
	const TaskCfg &m_cfg;
};


template<class TaskCfg>
class TaskMgr
{
public:
	using BaseTask_ = BaseTask<TaskCfg>;
	~TaskMgr();
	template<class DeriveTask, class... Args>
	bool RegTask(const TaskCfg &cfg, Args&&... args);
	bool UnRegTask(const TaskCfg &cfg);
	uint32 GetRegTaskNum() const;

	//事件
	//para ..., 每个参数的意义根据 GameTaskType在target_type_infos里面表达的意义来
	void Update(GameTaskType target_type, ...);
		
private:
	uint32 GetCfgPara(const TaskCfg &cfg, uint32 idx) const;
	//true表示 cur_num和cfg_last_para 符合逻辑关系 logic
	bool IsLogicOk(TaskParaLogic logic, int64 cfg_last_para, int64 cur_num) const;
	//判断非最后一个参数决定的任务条件是否符合
	//	para target_type_info 为 target_type_infos的一个元素
	//	para ..., Update传进来的可变参数。 格式为 para1, para2, para3 
	//return true表示符合条件
	bool IsMatchCondition(const GameTaskTypeDetail &target_type_info, const TaskCfg &task_cfg, va_list args) const;

private:
	using VecTask = std::vector<BaseTask_ *>;
	using Type2VecTask = std::map<GameTaskType, VecTask>;

	Type2VecTask m_type_2_vec_task;
};

/* 可变参数做完构造函数
template<class... _Valty>
void emplace_front(_Valty&&... _Val)
{	construct(
_STD forward<_Valty>(_Val)...);
}
*/
template<class TaskCfg>
template<class DeriveTask, class... Args>
bool TaskMgr<TaskCfg>::RegTask(const TaskCfg &cfg, Args&&... args)
{
	VecTask &vec = m_type_2_vec_task[(GameTaskType)cfg.task_type];
	for (BaseTask_ *task : vec)
	{
		if (cfg.id == task->GetCfg().id)
		{
			LOG_ERROR("repeated reg task.id=%d", cfg.id);
			return false;
		}
	}

	DeriveTask *p = new DeriveTask(cfg, std::forward<Args>(args)...);
	vec.push_back(p);
	return true;
}

template<class TaskCfg>
TaskMgr<TaskCfg>::~TaskMgr()
{
	for (auto &v : m_type_2_vec_task)
	{
		for (BaseTask_ *task : v.second)
		{
			delete task;
		}
	}
}


template<class TaskCfg>
bool TaskMgr<TaskCfg>::UnRegTask(const TaskCfg &cfg)
{
	auto it = m_type_2_vec_task.find((GameTaskType)cfg.task_type);
	if (it == m_type_2_vec_task.end())
	{
		return false;
	}
	VecTask &vec = it->second;
	for (BaseTask_ *task : vec)
	{
		if (cfg.id == task->GetCfg().id)
		{
			delete task;
			wyl::VecRemove(vec, task);
			return true;
		}
	}
	return false;
}

template<class TaskCfg>
uint32 TaskMgr<TaskCfg>::GetRegTaskNum() const
{
	uint32 num = 0;
	for (const auto &v : m_type_2_vec_task)
	{
		num += v.second.size();
	}
	return num;
}

template<class TaskCfg>
uint32 TaskMgr<TaskCfg>::GetCfgPara(const TaskCfg &cfg, uint32 idx) const
{
	switch (idx)
	{
	default:
		LOG_ERROR("idx too big. %d", idx); //target_type_info.vec_para_logic 定义的size过大
		return 0;
		break;
	case 0: return cfg.para1; break;
	case 1: return cfg.para2; break;
	case 2: return cfg.para3; break;
	case 3: return cfg.para4; break;
	case 4: return cfg.para5; break;
	}
}

template<class TaskCfg>
bool TaskMgr<TaskCfg>::IsLogicOk(TaskParaLogic logic, int64 cfg_last_para, int64 cur_num) const
{
	if (logic == TaskParaLogic::GREATER)
	{
		if (cur_num >= cfg_last_para)
		{
			return true;
		}
	}
	else if (logic == TaskParaLogic::LESS)
	{
		if (cur_num <= cfg_last_para)
		{
			return true;
		}
	}
	else if (logic == TaskParaLogic::EQUAL)
	{
		if (cur_num == cfg_last_para)
		{
			return true;
		}
	}
	else
	{
		LOG_ERROR("unknow logic %d", (int)logic);
		return false;
	}
	return false;
}

template<class TaskCfg>
void TaskMgr<TaskCfg>::Update(GameTaskType task_type, ...)
{
	va_list args;
	va_list forward_args; //转传给IsMatchCondition函数用
	va_start(args, task_type);
	va_start(forward_args, task_type);

	auto it = m_type_2_vec_task.find(task_type);
	if (it == m_type_2_vec_task.end())
	{
		return;
	}
	VecTask &vec_task = it->second;

	const auto it2 = GameTaskTypeMgr::Instance().GetCfg().find(task_type);
	if (it2 == GameTaskTypeMgr::Instance().GetCfg().end())
	{
		LOG_ERROR("unknow task type %d", (int)task_type);
		return;
	}
	const GameTaskTypeDetail &type_detail = it2->second;

	std::vector<typename VecTask::value_type> remove_task; //完成等删除的任务
	uint32 para_num = type_detail.vec_para_logic.size();
	if (para_num<1)
	{
		LOG_ERROR("error GameTaskTypeMgr::Instance().GetCfg(), type=%d", (int)task_type );
		return;
	}
	for (uint32 i = 0; i < para_num - 1; ++i)
	{
		va_arg(args, uint32);
	}
	const uint32 last_para = va_arg(args, uint32);

	//task_state 和 type_detail 比较，不符合条件退出, 符合改变进度
	for (BaseTask_ *base_task : vec_task)
	{
		const TaskCfg &task_cfg = base_task->GetCfg();

		//判断是否符合条件
		if (!IsMatchCondition(type_detail, task_cfg, forward_args))
		{
			break;
		}

		//符合条件，判断最后一个参数
		if (type_detail.is_last_para_absolute)//is_last_para_absolute true表示最后一个参数表示绝对值，false 表示累加值. default 表示累加
		{
			base_task->SetNum(last_para);
		}
		else
		{
			base_task->AddNum(last_para);
		}

		int64 cfg_last_para = GetCfgPara(task_cfg, para_num - 1);
		TaskParaLogic logic = type_detail.vec_para_logic.back();
		if (IsLogicOk(logic, cfg_last_para, base_task->GetNum()))
		{
			base_task->OnFinish();
			remove_task.push_back(base_task);
		}
		else
		{
			base_task->OnUpdate();
		}
	}

	for (const auto &v : remove_task)
	{
		delete v;
		wyl::VecRemove(vec_task, v);
	}
}

template<class TaskCfg>
bool TaskMgr<TaskCfg>::IsMatchCondition(const GameTaskTypeDetail &type_detail, const TaskCfg &task_cfg, va_list args) const
{
	int idx = 0;
	int last_idx = type_detail.vec_para_logic.size() - 1;
	uint32 cfg_para = 0;
	for (const auto &logic : type_detail.vec_para_logic)
	{
		if (idx == last_idx)//最后一个参数不判断
		{
			break;
		}
		cfg_para = GetCfgPara(task_cfg, idx);
		uint32 para = va_arg(args, uint32);
		if (!IsLogicOk(logic, cfg_para, para))
		{
			return false;
		}
		++idx;
	}
	return true;
}