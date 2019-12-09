
#include "game_task.h"





TaskMgr::~TaskMgr()
{
	for (auto &v : m_type_2_vec_task)
	{
		for (BaseTask_ *task : v.second)
		{
			delete task;
		}
	}
}



bool TaskMgr::UnRegTask(const TaskCfg &cfg)
{
	auto it = m_type_2_vec_task.find((TaskType)cfg.task_type);
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


uint32 TaskMgr::GetRegTaskNum() const
{
	uint32 num = 0;
	for (const auto &v : m_type_2_vec_task)
	{
		num += v.second.size();
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
	case 0: return cfg.para1; break;
	case 1: return cfg.para2; break;
	case 2: return cfg.para3; break;
	case 3: return cfg.para4; break;
	case 4: return cfg.para5; break;
	}
}


bool TaskMgr::IsLogicOk(TaskParaLogic logic, int64 cfg_last_para, int64 cur_num) const
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
		L_ERROR("unknow logic %d", (int)logic);
		return false;
	}
	return false;
}


void TaskMgr::Update(TaskType task_type, ...)
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
		L_ERROR("unknow task type %d", (int)task_type);
		return;
	}
	const GameTaskTypeDetail &type_detail = it2->second;

	std::vector<typename VecTask::value_type> remove_task; //完成等删除的任务
	uint32 para_num = type_detail.vec_para_logic.size();
	if (para_num>0)
	{
		for (int32 i = 0; i < (int32)para_num; ++i)
		{
			va_arg(args, uint32);
		}
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

		int64 cfg_last_para = GetCfgPara(task_cfg, para_num);
		TaskParaLogic logic = type_detail.finish_logic;
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


bool TaskMgr::IsMatchCondition(const GameTaskTypeDetail &type_detail, const TaskCfg &task_cfg, va_list args) const
{
	int idx = 0;
	int last_idx = type_detail.vec_para_logic.size() - 1;
	uint32 cfg_para = 0;
	for (const auto &logic : type_detail.vec_para_logic)
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