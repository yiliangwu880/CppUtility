#include "fsm_state.h"

StateMachine::StateMachine()
{
	m_p_last = nullptr;
	m_p_cur = &m_state1;
	m_p_cur->Enter();
}

void StateMachine::ChangeState(BaseState * m_state)
{
	m_p_last = m_p_cur;
	m_p_cur = m_state;
	m_p_last->Exit();
	m_p_cur->Enter();
}

void StateMachine::Event()
{
	m_p_cur->Event();
}

void State1::Event()
{
	//some condition call below
	m_machine->ChangeState(&m_machine->m_state2);
}
