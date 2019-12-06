/************************************************************************/
/*
author:YiliangWu
brief:
任务功能：
	通用地实现了往玩家身上加入任务，任务进度变化，结束功能。 不能直接复用，需要根据用户项目修改下配置字段。

待优化：
GameTaskTypeDetail配置 改为放到策划配置
反正都不不修改就复用，就不用模板，简化问题。直接定义TaskCfg，用户根据具体项目再修改 TaskCfg。 


新加任务需要做:
新任务类型，需要GameTaskTypeMgr构造函数里面加GameTaskTypeDetail配置. 
新任务， 策划配置按照 GameTaskTypeDetail 要求填值

看需要定义新任务类：比如class DeriveTask: public BaseTask
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
#include <algorithm>
#include "game_task_cfg.h"
#include <stdarg.h>
#include "../utility/stlBoost.h"


//策划配置模板，根据不同项目改命名
struct  TaskCfg
{
	uint32 id;
	uint32 task_type;
	uint32 para1, para2, para3, para4, para5; //选用参数
};

//派生类的构造函数，第一个参数必须为 const TaskCfg &
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



class TaskMgr
{
public:
	using BaseTask_ = BaseTask;
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

template<class DeriveTask, class... Args>
bool TaskMgr::RegTask(const TaskCfg &cfg, Args&&... args)
{
	VecTask &vec = m_type_2_vec_task[(GameTaskType)cfg.task_type];
	for (BaseTask_ *task : vec)
	{
		if (cfg.id == task->GetCfg().id)
		{
			L_ERROR("repeated reg task.id=%d", cfg.id);
			return false;
		}
	}

	DeriveTask *p = new DeriveTask(cfg, std::forward<Args>(args)...);
	vec.push_back(p);
	return true;
}