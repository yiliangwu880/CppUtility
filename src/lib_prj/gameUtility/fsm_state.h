/*
brief:
��Ч״̬���ο�����
*/

#pragma once
#include "../utility/typedef.h"

class StateObj //������ң�AI����
{
public:
private:

};

class StateMachine;
//״̬�ྡ���𱣴������Ϣ
class BaseState
{
public:
	virtual void Enter(){};
	virtual void Exit(){};
	virtual void Event(){};
	
protected:
	//StateObj &m_obj;
	StateMachine *m_machine;
};

class State1 : public BaseState
{
public:
	virtual void Event();
protected:
private:
};
class State2 : public BaseState
{
public:
protected:
private:
};

//״̬����򵥣����Կ���ȥ��StateMachine�࣬��״̬����������״̬��
class StateMachine
{
public:
	StateMachine();
	void ChangeState(BaseState * m_state);//ע�����ֻ��λ������ĳ�Ա֮һ��������Ķ�����
	void Event(); //���ݸ����¼�������״̬�����ԼӸ����¼��ӿ�

public:
	//����״̬�б�
	State1 m_state1;
	State2 m_state2;

private:
	BaseState *m_p_cur; //��ǰ״̬
	BaseState *m_p_last; //�����ʷ ״̬

};