/************************************************************************/
/*
author:YiliangWu
brief:
任务功能：
	通用地实现了往玩家身上加入任务，任务进度变化，结束功能。 不能直接复用，需要修改配置字段。
	如果需求一个任务，包含多个子任务项的情况，需要参考这个实现重构。

使用说明：
	可能需要根据项目修改下配置TaskCfg的字段名。
	配置：
	{
		新任务类型， 需要TaskType加类型， GetTaskType2Cfg函数里面加类型操作信息.
		新任务， 策划配置TaskCfg。
	}
	代码：
	{
		看需要定义新任务类：
		{
			比如class DeriveTask: public BaseTask
			实现任务进度变化回调函数。
			实现任务进度完成回调函数。
		}

		任务系统注册任务调用： TaskMgr::RegTask<DeriveTask>
		任务更新触发调用：TaskMgr::Update(task_type, ...)
	}

设置思想：
	本来配置信息可以做出模板，但代码会更复杂。这里选修改代码的方式使用，一个项目通常就一次修改，就能吧模板问题简化掉。
	

代码参考：
	class GetItemTask: public BaseTask
	{
	public:
		GetItemTask(const TaskCfg &cfg 这个参数必须放第一 , Player &player, int64 num = 0);
		virtual void OnFinish() override{};//可以做存档操作，比如存task_id, num, is_finish
		virtual void OnUpdate()override{};//可以做存档操作，比如存task_id, num
	}

	//注册任务
	TaskMgr mgr;
	Player player;
	TaskCfg cfg; 
	...
	mgr.RegTask<GetItemTask>(cfg, player, [1]);
	...
	//当触发任务改变事件发生时调用
	mgr:Update(TaskType::GET_ITEM, 1002, 1)；

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <map>
#include <algorithm>
#include <stdarg.h>
#include "../utility/stlBoost.h"


enum class TaskType
{
	KILL_MONSTER = 1,
	LV = 2,
	GET_QUALITY_ITEM = 3,
	GET_ITEM = 4,
};

//策划配置模板，根据不同项目改命名
struct  TaskCfg
{
	uint32 id;	                              //任务id,唯一就可以了
	uint32 task_type;		                  //GameTaskType
	uint32 para0, para1, para2, para3, para4; //选用参数
};

//任务基类
//派生类的构造函数，第一个参数必须为 const TaskCfg &
class BaseTask
{
private:
	int64 m_num;		  //表示进度
	const TaskCfg &m_cfg; //可以优化成读取静态配置，减少内存

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
};

//任务参数逻辑
enum class TaskParaOpt
{
	NONE,
	EQUAL = 0,      //TaskMgr::Update的参数 == 配置参数
	GREATER = 1,    //TaskMgr::Update的参数 >= 配置参数
	LESS = 2,       //TaskMgr::Update的参数 <= 配置参数
};

//配置所有任务类型
struct TaskTypeInfo
{
	//表达 TaskMgr::Update参数和任务配置参数的关系。 
	//vec_para_logic[0] 表示update参数1 para1的关系，  vec_para_logic[1]表示。。。。
	std::vector<TaskParaOpt> vec_para_opt;
	// task_state.num(任务进度) 和 TaskCfg.paraX 最后一个有效参数（达成目标）的关系。
	TaskParaOpt finish_opt;
	// true表示从Update传入的最后一个参数是绝对值，否则表示累加值.
	bool is_last_para_absolute;
};

class TaskMgr
{
public:
	~TaskMgr();
	template<class DeriveTask, class... Args>
	bool RegTask(const TaskCfg &cfg, Args&&... args);
	bool UnRegTask(const TaskCfg &cfg);
	uint32 GetRegTaskNum() const;

	//事件
	//para ..., 每个参数的意义根据 GameTaskType在target_type_infos里面表达的意义来
	void Update(TaskType target_type, ...);
		
private:
	uint32 GetCfgPara(const TaskCfg &cfg, uint32 idx) const;
	//true表示 cur_num和cfg_last_para 符合逻辑关系 logic
	bool IsLogicOk(TaskParaOpt logic, int64 cfg_last_para, int64 cur_num) const;
	//判断非最后一个参数决定的任务条件是否符合
	//	para target_type_info 为 target_type_infos的一个元素
	//	para ..., Update传进来的可变参数。 格式为 para1, para2, para3 
	//return true表示符合条件
	bool IsMatchCondition(const TaskTypeInfo &target_type_info, const TaskCfg &task_cfg, va_list args) const;

private:
	using VecTask = std::vector<BaseTask *>;
	using Type2VecTask = std::map<TaskType, VecTask>;

	Type2VecTask m_type_2_vec_task;
	bool m_is_enable_unreg = true; //放错误调用UnRegTask
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
	VecTask &vec = m_type_2_vec_task[(TaskType)cfg.task_type];
	for (BaseTask *task : vec)
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