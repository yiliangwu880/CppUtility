/************************************************************************/
/*
author:YiliangWu
brief:


任务功能：
	通用地实现了往玩家身上加入任务，任务进度变化，结束功能。 
	不能直接复用，需要修改配置字段。

扩展：
	如果需求一个任务，包含多个子任务项的情况，需要参考这个实现重构。
	{
		method1: 本系统加功能。配置参数为task id, Task加set保存一完成sub task. 全部完成算完成。
			comment:task系统复杂化了。
		method2: 新写MoreTaskMgr系统。 Task 完成通知 MoreTaskMgr, MoreTaskMgr触发MoreTask完成.
			comment:组合思想，划分模块，简化整个系统。不错
	}

术语：
	{
		Task：  代码层任务， 有类型，进度。     所有游戏任务，成就都是基于这个来实现
		Quest:	   游戏任务，可以关联一个Task实现，或者一个quest关联多个BaseTask， 全部BaseTask完成来计算完成。 
		Achieve:   成就，类似Quest。
		TaskMgr：  管理全部BaseTask，控制BaseTask状态变化。
	}

使用说明：
	需要根据项目修改下配置TaskCfg的字段名。
	配置：
	{
		新BaseTask类型， 需要加TaskType类型配置。 在代码 GetTaskType2Cfg 函数里面加.
		新Quest, 配置TaskCfg。
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
	本来配置信息可以做出模板，但代码会复杂。这里选修改代码的方式使用，一个项目通常就一次修改，就能把模板问题简化掉。
	

用户代码参考：
	class Player
	{
		public:
			TaskMgr m_task_mgr;
	}
	class GetItemTask: public BaseTask
	{
		Player &player;
	public:
		GetItemTask(const TaskCfg &cfg 这个参数必须放第一 , Player &player, int64 num = 0);
		virtual void OnFinish() override{};//可以做存档操作，比如存task_id, num, is_finish
		virtual void OnUpdate()override
		{
			player.mytask.num = GetNum();
		};
	}
	--------------------------
	//注册任务
	Player player;
	TaskMgr &mgr = player.m_task_mgr;
	TaskCfg cfg; 
	mgr.RegTask<GetItemTask>(cfg, player, [1]);
	--------------------------
	//当触发任务改变事件发生时调用
	mgr:Update(TaskType::GET_ITEM, 1002, 1)；

*/
/************************************************************************/

#pragma once

#include "../../svr_util/include/typedef.h"
#include <map>
#include <algorithm>
#include <array>
#include <vector>
#include <memory>
#include <stdarg.h>


//task类型，值作为数组索引
enum class TaskType
{
	KILL_MONSTER = 1,
	LV = 2,
	GET_QUALITY_ITEM = 3,
	GET_ITEM = 4,
	MAX_LEN,
};

//策划配置模板，根据不同项目改命名
//本来配置信息可以做出模板，会更通用， 但代码会更复杂。这里选修改代码的方式使用，一个项目通常就一次修改，就能吧模板问题简化掉。
struct  TaskCfg
{
	uint32 id;	                              //任务id,唯一就可以了
	uint32 task_type;		                  //TaskType
	uint32 para0, para1, para2, para3, para4; //选用参数
};

class TaskMgr;
//任务基类
//读库后也用构造初始化就可以了
//注意： 用户需要确保 cfg, 对象生存期比 BaseTask 对象长。 一般cfg一直到进程结束才释放
//  不能复制，因为构造对象就注册到TaskMgr了，避免临时对象注册到TaskMgr。
class BaseTask
{
private:
	int64 m_num;		  //表示进度
	std::shared_ptr<TaskMgr> m_pMgr;
	const TaskCfg &m_cfg; 

public:
	//@num 任务初始化进度
	BaseTask(const TaskCfg &cfg, std::shared_ptr<TaskMgr> pMgr, int64 num=0);

	virtual ~BaseTask();

	virtual void OnFinish() {};
	virtual void OnUpdate() {};

	const TaskCfg &GetCfg(){ return m_cfg; }
	void SetNum(int64 num){ m_num = num; }
	void AddNum(int64 add){ m_num += add; }
	const int64 GetNum(){ return m_num; }


	BaseTask(BaseTask&&)=default;
private:
	//noncopyable
	BaseTask(const BaseTask&);
	BaseTask & operator= (const BaseTask &);
};

//任务参数逻辑
enum class TaskParaOpt
{
	NONE,
	EQUAL = 0,      //TaskMgr::Update的参数 == 配置参数
	GREATER = 1,    //TaskMgr::Update的参数 >= 配置参数
	LESS = 2,       //TaskMgr::Update的参数 <= 配置参数
};

//任务类型
//表达 TaskMgr::Update参数和任务配置参数的关系。 
struct TaskTypeCfg
{
	//vec_para_logic[0] 表示TaskMgr::Update para0的关系，  vec_para_logic[1]表示。。。。
	//只有para0时，这里就为empty
	std::vector<TaskParaOpt> vec_para_opt;
	// task_state.num(任务进度) 和 TaskCfg.paraX 最后一个有效参数（达成目标）的关系。
	TaskParaOpt finish_opt;
	//表示从Update传入的最后一个参数使用方式。 false表示累加值. true表示绝对值，
	bool is_last_para_absolute;
};

//管理所有Task
//注意：TaskMgr free的时候，会free管理中的所有Task。
class TaskMgr
{
	friend class BaseTask;
private:
	using VecTask = std::vector<BaseTask *>;
	using VecTaskS = std::array<VecTask, (uint32)TaskType::MAX_LEN>; 

	VecTaskS m_all_task;	 //type idx VecTask
	bool m_is_updateing = false; //防错误调用

public:
	~TaskMgr();
	uint32 GetRegTaskNum() const;

	//更新事件，如果BaseTask 完成，会delete这个对象
	//@para ..., 每个参数的意义根据 TaskType, TaskTypeCfg 意义来决定
	//注意：每个参数必需为类型uint32
	void Update(TaskType target_type, ...);	
	
	//true表示 进度完成
	bool IsFinish(const TaskCfg &task_cfg, uint32 cur_num);
		
private:
	bool RegTask(BaseTask &task);
	bool UnRegTask(BaseTask &task);
	uint32 GetCfgPara(const TaskCfg &cfg, uint32 idx) const;
	//true表示 cur_num,cfg_para 符合逻辑关系 logic
	bool IsLogicOk(TaskParaOpt logic, int64 cur_num, int64 cfg_para) const;
	//判断任务条件是否符合, 根据 TaskTypeCfg， TaskCfg 和 参数列表比对，排除最后一个参数。
	//@target_type_info	为 TaskTypeCfg的一个元素
	//@args		Update传进来的可变参数。 格式为 para1, para2, para3 
	//return true表示符合条件
	bool IsMatchCondition(const TaskTypeCfg &target_type_info, const TaskCfg &task_cfg, va_list args) const;

};
