/************************************************************************/
/*              
author:YiliangWu
brief:��������

*/
/************************************************************************/

#pragma once

#include "../utility/typedef.h"
#include <map>
#include <vector>

enum class TaskType
{
	KILL_MONSTER     = 1,
	LV               = 2,
	GET_QUALITY_ITEM = 3,
	GET_ITEM         = 4,
};

//��������߼�
enum class TaskParaLogic
{
	NONE,
	EQUAL   = 0,    //==
	GREATER = 1,    //>=
	LESS    = 2,    //<=
};

//����������������
struct GameTaskTypeDetail
{
	std::vector<TaskParaLogic> vec_para_logic;	//��� Update�������������ò����Ĺ�ϵ�� 
	TaskParaLogic finish_logic;			//������Ⱥ� TaskCfg.paraX ���һ����Ч���������Ŀ�꣩�Ĺ�ϵ��
	bool is_last_para_absolute;			// true��ʾ��Update��������һ�������Ǿ���ֵ�������ʾ�ۼ�ֵ.
};

struct  GameTaskLogicCfg
{
	//��� Update�������������ò����Ĺ�ϵ�� 
	//vec_para_logic[0] ��ʾupdate����1 para1�Ĺ�ϵ��  vec_para_logic[1]��ʾ��������
	std::vector<std::string> vec_para_logic;
	// task_state.num(�������) �� TaskCfg.paraX ���һ����Ч���������Ŀ�꣩�Ĺ�ϵ�� 
	std::string finish_logic;
	// true��ʾ��Update��������һ�������Ǿ���ֵ�������ʾ�ۼ�ֵ.
	bool is_last_para_absolute;
};
using TaskType2Cfg = std::map<TaskType, GameTaskLogicCfg>;

//��ȡ����
class GameTaskTypeMgr
{
private:
	std::map<TaskType, GameTaskTypeDetail> m_cfg;
public:
	static const GameTaskTypeMgr &Instance()
	{
		static GameTaskTypeMgr d;
		return d;
	}
	const std::map<TaskType, GameTaskTypeDetail> &GetCfg() const { return m_cfg; }
private:
	static const TaskType2Cfg &GetTaskType2Cfg()
	{
		static TaskType2Cfg cfg=
		{
			{//para1==update::para1��������� >= para2 ��Update��������һ���������ۼ�ֵ
				TaskType::KILL_MONSTER, 
				{{"==",}, ">=", false,}
			},
			{//������� >= para1 ��Update��������һ���������ۼ�ֵ
				TaskType::LV, 
				{{}, ">=", true,}
			},					
			{//��ȡid��Ʒ�� >= Ʒ�ʣ� >= para2
				TaskType::GET_QUALITY_ITEM,
				{{"==", ">="}, ">=", false,}
			},
			{
				TaskType::GET_ITEM,
				{{"=="}, ">=", false,}
			},
		};
		return cfg;
	}
	TaskParaLogic Str2TaskParaLogic(const std::string &s)
	{
		if (s == "==")
		{
			return TaskParaLogic::EQUAL;
		}
		else if (s == ">=")
		{
			return TaskParaLogic::GREATER;
		}
		else if (s == "<=")
		{
			return TaskParaLogic::LESS;
		}
		else
		{
			L_ERROR("unknow logic type %s", s.c_str());
			return TaskParaLogic::NONE;
		}
	}
	GameTaskTypeMgr()
	{
		//�ַ�������ת�� ���ã��ӿ�Ч��
		const TaskType2Cfg &str_cfg= GameTaskTypeMgr::GetTaskType2Cfg();
		for (const auto &v: str_cfg)
		{
			const GameTaskLogicCfg &cfg = v.second;
			GameTaskTypeDetail &m_cfg_v = m_cfg[v.first];
			for (const std::string &s : cfg.vec_para_logic)
			{
				m_cfg_v.vec_para_logic.push_back(Str2TaskParaLogic(s));
			}
			m_cfg_v.finish_logic = Str2TaskParaLogic(cfg.finish_logic);
			m_cfg_v.is_last_para_absolute = cfg.is_last_para_absolute;
		}
	}

};
