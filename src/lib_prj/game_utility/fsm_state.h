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

		//特点： 状态类名 就当类似枚举用，免去同样意义多处重复定义。
		template<class State>
		bool IsState()
		{
			State *p = dynamic_cast<State *>(m_state);
			return  (NULL != p);
		}
	};

}
