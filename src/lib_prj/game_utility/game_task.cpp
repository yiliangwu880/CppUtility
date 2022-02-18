
#include "game_task.h"

using namespace std;
//意义和 TaskTypeCfg 一样，为了容易人工编写
struct  TaskTypeStrInfo
{
	std::vector<std::string> vec_para_opt ;
	std::string finish_opt ;
	bool is_last_para_absolute ;
};

using TaskType2Cfg = std::map<TaskType, TaskTypeStrInfo>;


namespace {
	const TaskType2Cfg & GetTaskType2Cfg()
	{
		static  TaskType2Cfg cfg=
		{			
			{//para0==update::para0，任务进度 >= para1 ，para1是累加值
				TaskType::KILL_MONSTER,
				{{"==",}, ">=", false,}
			},
			{//任务进度 >= para0 ，para0是绝对值
				TaskType::LV,
				{{}, ">=", true}
			},
			{//获取id物品， >= 品质， >= para2
				TaskType::GET_QUALITY_ITEM,
				{{"==", ">="},">=", false,}
			},
			{
				TaskType::GET_ITEM,
				{{"=="},">=", false,}
			},
		};
		return cfg;
	}
}


template <typename Vec>
inline bool VecRemove(Vec& vec, typename Vec::value_type val)
{
	for (typename Vec::iterator iter = vec.begin(); iter != vec.end(); ++iter)
	{
		if (*iter == val)
		{
			*iter = vec.back();
			vec.erase(vec.end() - 1);
			return true;
		}
	}
	return false;
}



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
		TaskTypeCfg &m_cfg_v = m_cfg[(uint32_t)t];
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
	for (const VecTask &v : m_all_task)
	{
		for (BaseTask *task : v)
		{
			delete task;
		}
	}
}



bool TaskMgr::DestoryTask(uint64 id)
{//注意：别调用 task 的虚函数
	if (m_is_updateing)
	{
		L_ERROR("error call UnRegTask")
		return false;
	}

	for (VecTask &vec : m_all_task)
	{
		for (BaseTask *pTask : vec)
		{
			if ( id == (uint64)pTask)
			{
				VecRemove(vec, pTask);
				delete pTask;
				return true;
			}
		}
	}
	
	//注销失败有时候是正常的，因为任务完成的时候就已经注销了。
	return false;
}

const BaseTask * TaskMgr::Find(TaskType type, uint64 id)
{
	if ((size_t)type >= (size_t)TaskType::MAX_LEN)
	{
		L_ERROR("unknow task_type %lld", (size_t)type);
		return nullptr;
	}
	const VecTask &vec = m_all_task[(size_t)type];
	for (const BaseTask *pTask : vec)
	{
		if (id == (uint64)pTask)
		{
			return pTask;
		}
	}
	return nullptr;
}

uint32 TaskMgr::GetTaskNum() const
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


bool TaskMgr::IsLogicOk(TaskParaOpt logic, int64 cur_num, int64 cfg_para) const
{
	if (logic == TaskParaOpt::GREATER)
	{
		if (cur_num >= cfg_para)
		{
			return true;
		}
	}
	else if (logic == TaskParaOpt::LESS)
	{
		if (cur_num <= cfg_para)
		{
			return true;
		}
	}
	else if (logic == TaskParaOpt::EQUAL)
	{
		if (cur_num == cfg_para)
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


bool TaskMgr::IsFinish(const TaskCfg &task_cfg, uint32 cur_num)
{
	const auto &ar = GameTaskTypeMgr::Ins().GetCfg();
	if (task_cfg.task_type >= ar.size())
	{
		L_ERROR("task type overflow %d", task_cfg.task_type);
		return false;
	}
	const TaskTypeCfg &type_cfg = ar[task_cfg.task_type];

	int64 cfg_last_para = GetCfgPara(task_cfg, type_cfg.vec_para_opt.size());
	return IsLogicOk(type_cfg.finish_opt, cur_num, cfg_last_para);
}





BaseTask::BaseTask(const TaskCfg &cfg, int64 num)
	:m_num(num)
	, m_cfg(cfg)
{



}

BaseTask::~BaseTask()
{

}
