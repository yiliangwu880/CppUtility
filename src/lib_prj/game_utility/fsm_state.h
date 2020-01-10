/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/CppUtility.git
brief: finite-state machine
����״̬���ο�����


*/

#pragma once
#include "../utility/typedef.h"

namespace StateMachine
{
	class Obj;
	struct BaseState
	{
		BaseState(Obj &obj)
			:m_obj(obj)
		{}
		Obj &m_obj;

		virtual void OnStart() {};
		virtual void OnEnd() {};
		//add you function, like :virtual void Handle1();
	};
	struct State1 : public BaseState
	{
		State1(Obj &obj)
			:BaseState(obj)
		{}
	};
	struct State2 : public BaseState
	{
		State2(Obj &obj)
			:BaseState(obj)
		{}
	};

	class Obj
	{
		BaseState *m_state = nullptr;
		State1 m_s1;
		State1 m_s2;
	public:
		void ChangeState(BaseState &state)
		{
			m_state->OnEnd();
			m_state = &state;
			m_state->OnStart();
		}

		//�ص㣺 ״̬���� �͵�����ö���ã���ȥͬ������ദ�ظ����塣
		template<class State>
		bool IsState()
		{
			State *p = dynamic_cast<State *>(m_state);
			return  (NULL != p);
		}
	};

}
