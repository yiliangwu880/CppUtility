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

enum class GameTaskType
{
	KILL_MONSTER = 1,
	LV = 2,
	GET_QUALITY_ITEM = 3,
	GET_ITEM = 4,
};

//��������߼�
enum class TaskParaLogic
{
	EQUAL = 0,
	GREATER = 1,
	LESS = 2,
};

//����������������
struct GameTaskTypeDetail
{
	//��� Update�������������ò����Ĺ�ϵ��ȫ��Update����(�ų����һ��)�������ò����Ĺ�ϵ������ƥ�����������
	//���һ������ ��� task_state.num(����)  �� ���ò���para�Ĺ�ϵ, ����������ʾ��ɡ�
	std::vector<TaskParaLogic> vec_para_logic; 
	// true��ʾ��Update��������һ�������Ǿ���ֵ�������ʾ�ۼ�ֵ.
	bool is_last_para_absolute; 
};
//��ȡ���õ���
class GameTaskTypeMgr
{
public:
	static const GameTaskTypeMgr &Instance()
	{
		static GameTaskTypeMgr d;
		return d;
	}
	const std::map<GameTaskType, GameTaskTypeDetail> &GetCfg() const { return m_cfg; }
private:
	GameTaskTypeMgr()
	{
		m_cfg[GameTaskType::KILL_MONSTER] = 
		{
			{ TaskParaLogic::EQUAL, TaskParaLogic::GREATER },		//ɱid�֣� >= para2 
			//true, //���ȱʡֵΪis_last_para_absolute = false
		};
		m_cfg[GameTaskType::LV] =
		{
			{ TaskParaLogic::GREATER },		//�ȼ� >= para1
			true,
		};
		m_cfg[GameTaskType::GET_QUALITY_ITEM] =
		{
			{ TaskParaLogic::EQUAL, TaskParaLogic::GREATER, TaskParaLogic::GREATER },		//��ȡid��Ʒ�� >= Ʒ�ʣ� >= para2
		};
		m_cfg[GameTaskType::GET_ITEM] =
		{
			{ TaskParaLogic::EQUAL, TaskParaLogic::GREATER },		//��ȡid��Ʒ�� >= para2
		};
	}

private:
	std::map<GameTaskType, GameTaskTypeDetail> m_cfg;
};