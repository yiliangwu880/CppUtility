
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
//任务类型信息配置
class GameTaskTypeMgr
{
private:
	std::array<TaskTypeCfg, (uint32_t)TaskType::MAX_LEN> m_cfg;

public:
	static const GameTaskTypeMgr &Obj()
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

void TaskMgr::Update(TaskType task_type, ...)
{
	//m_is_updateing 控制，保证了m_all_task 在Update中不会被别的地方 删除，增加元素
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

	if ((uint32_t)task_type >= m_all_task.size() || (uint32_t)task_type >= GameTaskTypeMgr::Obj().GetCfg().size())
	{
		m_is_updateing = false;
		return;
	}
	VecTask &vec_task = m_all_task[(uint32_t)task_type];
	const TaskTypeCfg &type_cfg = GameTaskTypeMgr::Obj().GetCfg()[(uint32_t)task_type];


	uint32 para_num = type_cfg.vec_para_opt.size();
	if (para_num>0)
	{
		for (int32 i = 0; i < (int32)para_num; ++i)
		{
			va_arg(args, uint32);
		}
	}
	const uint32 last_para = va_arg(args, uint32);

	VecTask remove_task; //完成等删除的task
	//不符合条件退出, 符合改变进度
	for (BaseTask *base_task : vec_task)
	{
		const TaskCfg &task_cfg = base_task->GetCfg();
		//判断是否符合条件
		if (!IsMatchCondition(type_cfg, task_cfg, forward_args))
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

		if(IsFinish(task_cfg, base_task->GetNum()))
		{
			base_task->OnFinish();
			remove_task.push_back(base_task);
		}
		else
		{
			base_task->OnUpdate();
		}
	}

	for (BaseTask * v : remove_task)
	{
		VecRemove(vec_task, v);
		delete v;
	}
	m_is_updateing = false;
}

bool TaskMgr::IsFinish(const TaskCfg &task_cfg, uint32 cur_num)
{
	const auto &ar = GameTaskTypeMgr::Obj().GetCfg();
	if (task_cfg.task_type >= ar.size())
	{
		L_ERROR("task type overflow %d", task_cfg.task_type);
		return false;
	}
	const TaskTypeCfg &type_cfg = ar[task_cfg.task_type];

	int64 cfg_last_para = GetCfgPara(task_cfg, type_cfg.vec_para_opt.size());
	return IsLogicOk(type_cfg.finish_opt, cur_num, cfg_last_para);
}

bool TaskMgr::IsMatchCondition(const TaskTypeCfg &type_detail, const TaskCfg &task_cfg, va_list args) const
{
	int idx = 0;
	int last_idx = type_detail.vec_para_opt.size() - 1;
	uint32 cfg_para = 0;
	for (const auto &logic : type_detail.vec_para_opt)
	{
		cfg_para = GetCfgPara(task_cfg, idx);
		uint32 para = va_arg(args, uint32);
		if (!IsLogicOk(logic, para, cfg_para))
		{
			return false;
		}
		++idx;
	}
	return true;
}



BaseTask::BaseTask(const TaskCfg &cfg, int64 num)
	:m_num(num)
	, m_cfg(cfg)
{



}

BaseTask::~BaseTask()
{

}
