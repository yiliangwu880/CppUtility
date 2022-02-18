/************************************************************************/
/*
you can get more refer from https://github.com/yiliangwu880/CppUtility.git
author:YiliangWu
brief:
任务功能：
	通用地实现了往玩家身上加入Task，Task进度变化，Task完成。 
	不能直接复用，需要修改配置字段。

注意： 
	对象 用了 引用或者指针，要注意保证被引用对象的生存期要长。
	初步本版建议保险做法： 用ID查找对象，不要直接引用。
术语：
	{
		Task：	   作业， 有类型，进度。     所有游戏任务，成就都是基于这个来实现
		Quest:	   游戏任务，可以关联一个Task实现，或者一个quest关联多个BaseTask， 全部BaseTask完成来计算完成。
		Achieve:   成就，类似Quest。
		TaskMgr：  管理全部BaseTask，控制BaseTask状态变化。
	}

待用户扩展：
	如果需求一个任务，包含多个Task项的情况，需要参考下面思想实现。
	新写MoreTaskMgr系统。 Task 完成通知 MoreTaskMgr, MoreTaskMgr触发MoreTask完成.
	优点： 组合思想，划分模块，简化整个系统。

其他：
	设计思想，本来配置信息可以做出模板，但代码会复杂。这里选修改代码的方式使用，一个项目通常就一次修改，就能把模板问题简化掉。

使用说明：
	1) 根据项目修改下配置TaskCfg的字段名。
	2) 给所有TaskType值加逻辑配置。 表达 task进度更新事件参数和TaskCfg的逻辑关系。  GetTaskType2Cfg 函数里面加。

用户代码参考：
	class MyTask: public BaseTask
	{
		Player &player;
	public:
		MyTask(const TaskCfg &cfg 这个参数必须放第一 , Player &player, int64 num = 0);
		virtual void OnFinish() override
		{
			player.m_Question[GetId()].status = finish.
		};

		virtual void OnUpdate()override
		{
			player.m_Question[GetId()] = GetNum();
		};
	}
	class Player
	{
		struct Question
		{
			int status;
			int num;
		}
		public:
			TaskMgr m_task_mgr; 
			map<uint64, Question> m_Question;
	}

	--------------------------
	//创建任务
	Player player;
	TaskMgr &mgr = player.m_task_mgr;
	TaskCfg cfg; 
	uint64 id = player.mgr.RegTask<MyTask>(cfg, player, [1]);

	--------------------------
	//当触发任务改变事件发生时调用
	mgr:Update(TaskType::GET_ITEM, 1002, 1)；

	--------------------------
	//注销任务
	player.mgr.UnRegTask(id);
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
class BaseTask
{
private:
	int64 m_num;		  //表示进度
	const TaskCfg &m_cfg; 

public:
	//@num 任务初始化进度
	BaseTask(const TaskCfg &cfg,  int64 num=0);

	virtual ~BaseTask();

	virtual void OnFinish()=0;
	virtual void OnUpdate()=0;

	const TaskCfg &GetCfg(){ return m_cfg; }
	void SetNum(int64 num){ m_num = num; }
	void AddNum(int64 add){ m_num += add; }
	const int64 GetNum(){ return m_num; }
	uint64 GetId() { return (uint64)this; }

private:
	//noncopyable
	//  不能复制，因为构造对象就注册到TaskMgr了，避免临时对象注册到TaskMgr。
	//  不能存放vector当元素，重分配内存导致对象地址变化了。
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

//任务类型配置
//表达 task进度更新事件参数和TaskCfg的逻辑关系。 
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
	uint32 GetTaskNum() const;

	//创建Task,避免了用户操心动态分配
	//return indicate fail
	template<class DeriveTask, class... Args>
	uint64 CreateTask(const TaskCfg &cfg, Args&&... args);
	bool DestoryTask(uint64 id);
	bool DestoryTask(TaskType type, uint64 id); //效率更高， unfinish
	const BaseTask *Find(TaskType type, uint64 id); 

	//@brief (process update event) task进度更新事件。如果 Task 完成，会被注销
	//@para ..., 每个参数的意义根据 TaskType, TaskTypeCfg 意义来决定
	//注意：每个参数必需为类型uint32
	template<class ... Args>
	void Update(TaskType task_type, Args&& ... args);
	
	//true表示 进度完成
	bool IsFinish(const TaskCfg &task_cfg, uint32 cur_num);
		
private:
	uint32 GetCfgPara(const TaskCfg &cfg, uint32 idx) const;
	//true表示 cur_num,cfg_para 符合逻辑关系 logic
	bool IsLogicOk(TaskParaOpt logic, int64 cur_num, int64 cfg_para) const;
	//判断任务条件是否符合, 根据 TaskTypeCfg， TaskCfg 和 参数列表比对，排除最后一个参数。
	//@target_type_info	为 TaskTypeCfg的一个元素
	//@args		Update传进来的可变参数。 格式为 para1, para2, para3 
	//return true表示符合条件
	template<class ... Args>
	bool IsMatchCondition(const TaskTypeCfg &target_type_info, const TaskCfg &task_cfg, Args&& ... args) const;


};

//任务类型信息配置
class GameTaskTypeMgr
{
private:
	std::array<TaskTypeCfg, (uint32_t)TaskType::MAX_LEN> m_cfg;

public:
	static const GameTaskTypeMgr &Ins()
	{
		static GameTaskTypeMgr d;
		return d;
	}
	const std::array<TaskTypeCfg, (uint32_t)TaskType::MAX_LEN> &GetCfg() const { return m_cfg; }
	bool IsCfgOk();//检查配置非法

private:
	TaskParaOpt Str2TaskParaLogic(const std::string &s);
	GameTaskTypeMgr();
};

//注册Task,避免了用户操心动态分配
template<class DeriveTask, class... Args>
uint64 TaskMgr::CreateTask(const TaskCfg &cfg, Args&&... args)
{
	if (m_is_updateing)
	{
		L_ERROR("error call regTask during updateing");
		return 0;
	}
	TaskType t = (TaskType)cfg.task_type;
	if ((uint32_t)t >= (uint32_t)TaskType::MAX_LEN)
	{
		L_ERROR("illegal task_type=%d", cfg.task_type);
		return 0;
	}
	VecTask &vec = m_all_task[(uint32_t)t];
	DeriveTask *p = new DeriveTask(cfg, std::forward<Args>(args)...);
	vec.push_back(p);
	return (uint64)p;
}

template<class ... Args>
void TaskMgr::Update(TaskType task_type, Args&& ... args)
{
	//m_is_updateing 控制，保证了m_all_task 在Update中不会被别的地方 删除，增加元素
	if (m_is_updateing)
	{
		L_ERROR("recursive call");
		return;
	}
	const TaskTypeCfg &type_cfg = GameTaskTypeMgr::Ins().GetCfg()[(uint32_t)task_type];
	if (sizeof...(Args) != type_cfg.vec_para_opt.size() + 1)
	{
		L_ERROR("error para num");
		return;
	}
	m_is_updateing = true;

	if ((uint32_t)task_type >= m_all_task.size() || (uint32_t)task_type >= GameTaskTypeMgr::Ins().GetCfg().size())
	{
		m_is_updateing = false;
		return;
	}
	VecTask &vec_task = m_all_task[(uint32_t)task_type];


	uint32 last_para = 0;
	auto GetPara = [&last_para](uint32 para)
	{
		last_para = para;
	};
	std::initializer_list<int> { (GetPara(std::forward<Args>(args)), 0)... };
	VecTask remove_task; //完成等删除的task
	//不符合条件退出, 符合改变进度
	for (BaseTask *base_task : vec_task)
	{
		const TaskCfg &task_cfg = base_task->GetCfg();
		//判断是否符合条件
		if (!IsMatchCondition(type_cfg, task_cfg, std::forward<Args>(args)...))
		{
			continue;
		}

		//符合条件，判断最后一个参数
		if (type_cfg.is_last_para_absolute)//is_last_para_absolute true表示最后一个参数表示绝对值，false 表示累加值. default 表示累加
		{
			base_task->SetNum(last_para);
		}
		else
		{
			base_task->AddNum(last_para);
		}

		if (IsFinish(task_cfg, base_task->GetNum()))
		{
			remove_task.push_back(base_task);
		}
		else
		{
			base_task->OnUpdate();
		}
	}

	m_is_updateing = false;
	for (BaseTask * v : remove_task)
	{
		v->OnFinish();
		//VecRemove(vec_task, v);
		for (auto iter = vec_task.begin(); iter != vec_task.end(); ++iter)
		{
			if (*iter == v)
			{
				*iter = vec_task.back();
				vec_task.erase(vec_task.end() - 1);
				break;
			}
		}
		delete v;
	}
}

template<class ... Args>
bool TaskMgr::IsMatchCondition(const TaskTypeCfg &type_detail, const TaskCfg &task_cfg, Args&& ... args) const
{
	if (sizeof...(Args) != type_detail.vec_para_opt.size() + 1)
	{
		L_ERROR("error para num");
		return false;
	}
	int idx = 0;
	std::vector<uint32> vec_para;
	auto f = [&vec_para](uint32 para)
	{
		vec_para.push_back(para);
	};
	std::initializer_list<int>{(f(std::forward<Args>(args)), 0)...};
	for (const auto &logic : type_detail.vec_para_opt)
	{
		uint32 cfg_para = GetCfgPara(task_cfg, idx);
		uint32 para = vec_para[idx];
		if (!IsLogicOk(logic, para, cfg_para))
		{
			return false;
		}
		++idx;
	}
	return true;
}
