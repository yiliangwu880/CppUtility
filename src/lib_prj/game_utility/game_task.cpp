
#include "game_task.h"

//意义和TaskTypeInfo一样，为了容易人工编写
struct  TaskTypeStrInfo
{
	std::vector<std::string> vec_para_opt;
	std::string finish_opt= ">=";
	bool is_last_para_absolute = false;
};

using TaskType2Cfg = std::map<TaskType, TaskTypeStrInfo>;

namespace {
	const TaskType2Cfg & GetTaskType2Cfg()
	{
		static const TaskType2Cfg cfg =
		{
			{//para0==update::para0，任务进度 >= para1 ，para1是累加值
				TaskType::KILL_MONSTER,
				{{"==",},/* ">=", false,*/}
			},
			{//任务进度 >= para0 ，para0是绝对值
				TaskType::LV,
				{{}, ">=", true,}
			},
			{//获取id物品， >= 品质， >= para2
				TaskType::GET_QUALITY_ITEM,
				{{"==", ">="},}
			},
			{
				TaskType::GET_ITEM,
				{{"=="},}
			},
		};
		return cfg;
	}
}


//任务类型信息配置
class GameTaskTypeMgr
{
private:
	std::array<TaskTypeInfo, TaskType::MAX_LEN> m_cfg;

public:
	static const GameTaskTypeMgr &Instance()
	{
		static GameTaskTypeMgr d;
		return d;
	}
	const std::array<TaskTypeInfo, TaskType::MAX_LEN> &GetCfg() const { return m_cfg; }
	bool IsCfgOk();//检查配置非法

private:
	TaskParaOpt Str2TaskParaLogic(const std::string &s);
	GameTaskTypeMgr();
};



bool GameTaskTypeMgr::IsCfgOk()
{
	const TaskType2Cfg &str_cfg = GetTaskType2Cfg();
	for (const auto &v : str_cfg)
	{
		const TaskTypeStrInfo &cfg = v.second;
		for (const std::string &s : cfg.vec_para_opt)
		{
			if (TaskParaOpt::NONE == Str2TaskParaLogic(s))
			{
				return false;
			}
		}

		if (TaskParaOpt::NONE == Str2TaskParaLogic(cfg.finish_opt))
		{
			return false;
		}
	}
	return true;
}

TaskParaOpt GameTaskTypeMgr::Str2TaskParaLogic(const std::string &s)
{
	if (s == "==")
	{
		return TaskParaOpt::EQUAL;
	}
	else if (s == ">=")
	{
		return TaskParaOpt::GREATER;
	}
	else if (s == "<=")
	{
		return TaskParaOpt::LESS;
	}
	else
	{
		L_ERROR("unknow logic type %s", s.c_str());
		return TaskParaOpt::NONE;
	}
}


GameTaskTypeMgr::GameTaskTypeMgr()
{
	//字符串配置转换 配置，加快效率
	const TaskType2Cfg &str_cfg = GetTaskType2Cfg();
	for (const auto &v : str_cfg)
	{
		TaskType t = v.first;
		if (t >= TaskType::MAX_LEN)
		{
			L_ERROR("unknow task_type %d", t);
			continue;
		}
		const TaskTypeStrInfo &cfg = v.second;
		TaskTypeInfo &m_cfg_v = m_cfg[t];
		for (const std::string &s : cfg.vec_para_opt)
		{
			m_cfg_v.vec_para_opt.push_back(Str2TaskParaLogic(s));
		}
		m_cfg_v.finish_opt = Str2TaskParaLogic(cfg.finish_opt);
		m_cfg_v.is_last_para_absolute = cfg.is_last_para_absolute;
	}
}

TaskMgr::~TaskMgr()
{
	for (auto &v : m_all_task)
	{
		for (BaseTask *task : v.second)
		{
			delete task;
		}
	}
}



bool TaskMgr::UnRegTask(const TaskCfg &cfg)
{
	if (m_is_updateing)
	{
		L_ERROR("error call UnRegTask")
		return false;
	}
	if (cfg.task_type>=TaskType::MAX_LEN)
	{
		L_ERROR("unknow task_type %d", cfg.task_type);
		return false;
	}
	VecTask &vec = m_all_task[(TaskType)cfg.task_type];
	for (BaseTask *task : vec)
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


uint32 TaskMgr::GetRegTaskNum() const
{
	uint32 num = 0;
	for (const auto &v : m_all_task)
	{
		num += v.size();
	}
	return num;
}


uint32 TaskMgr::GetCfgPara(const TaskCfg &cfg, uint32 idx) const
{
	switch (idx)
	{
	default:
		L_ERROR("idx too big. %d", idx); //target_type_info.vec_para_logic 定义的size过大
		return 0;
		break;
	case 0: return cfg.para0; break;
	case 1: return cfg.para1; break;
	case 2: return cfg.para2; break;
	case 3: return cfg.para3; break;
	case 4: return cfg.para4; break;
	}
}


bool TaskMgr::IsLogicOk(TaskParaOpt logic, int64 cfg_last_para, int64 cur_num) const
{
	if (logic == TaskParaOpt::GREATER)
	{
		if (cur_num >= cfg_last_para)
		{
			return true;
		}
	}
	else if (logic == TaskParaOpt::LESS)
	{
		if (cur_num <= cfg_last_para)
		{
			return true;
		}
	}
	else if (logic == TaskParaOpt::EQUAL)
	{
		if (cur_num == cfg_last_para)
		{
			return true;
		}
	}
	else
	{
		L_ERROR("unknow logic %d", (int)logic);
		return false;
	}
	return false;
}


void TaskMgr::Update(TaskType task_type, ...)
{
	if (m_is_updateing)
	{
		L_ERROR("recurrive call");
		return;
	}
	m_is_updateing = true;
	va_list args;
	va_list forward_args; //转传给IsMatchCondition函数用
	va_start(args, task_type);
	va_start(forward_args, task_type);

	if (task_type>=TaskType::MAX_LEN)
	{
		m_is_updateing = false;
		return;
	}
	VecTask &vec_task = m_all_task[task_type];

	//待优化
	//baseTask初始化的时候，绑定TaskTypeInfo *type_detail指针,不必每次update查
	const TaskTypeInfo *type_info = wyl::MapFind(GameTaskTypeMgr::Instance().GetCfg(), task_type);
	if (nullptr == type_info)
	{
		L_ERROR("unknow task type %d", (int)task_type);
		m_is_updateing = false;
		return;
	}

	std::vector<typename VecTask::value_type> remove_task; //完成等删除的任务
	uint32 para_num = type_info->vec_para_opt.size();
	if (para_num>0)
	{
		for (int32 i = 0; i < (int32)para_num; ++i)
		{
			va_arg(args, uint32);
		}
	}
	const uint32 last_para = va_arg(args, uint32);

	//task_state 和 type_detail 比较，不符合条件退出, 符合改变进度
	for (BaseTask *base_task : vec_task)
	{
		const TaskCfg &task_cfg = base_task->GetCfg();

		//判断是否符合条件
		if (!IsMatchCondition(*type_info, task_cfg, forward_args))
		{
			break;
		}

		//符合条件，判断最后一个参数
		if (type_info->is_last_para_absolute)//is_last_para_absolute true表示最后一个参数表示绝对值，false 表示累加值. default 表示累加
		{
			base_task->SetNum(last_para);
		}
		else
		{
			base_task->AddNum(last_para);
		}

		int64 cfg_last_para = GetCfgPara(task_cfg, para_num);
		if (IsLogicOk(type_info->finish_opt, cfg_last_para, base_task->GetNum()))
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
	m_is_updateing = false;
}


bool TaskMgr::IsMatchCondition(const TaskTypeInfo &type_detail, const TaskCfg &task_cfg, va_list args) const
{
	int idx = 0;
	int last_idx = type_detail.vec_para_opt.size() - 1;
	uint32 cfg_para = 0;
	for (const auto &logic : type_detail.vec_para_opt)
	{
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


