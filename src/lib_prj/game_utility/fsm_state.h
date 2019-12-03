/*
author:yiliangwu880
you can get more refer from https://github.com/yiliangwu880/CppUtility.git
brief: finite-state machine
有限状态机参考代码


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
	public:
		BaseState *m_state = nullptr;

		template<class State>
		void ChangeState()
		{
			if (nullptr != m_state)
			{
				delete m_state;
			}
			m_state = new State(*this);
		}

		template<class State>
		bool IsState()
		{
			State *p = dynamic_cast<State *>(m_state);
			return  (NULL != p);
		}
	};

}
