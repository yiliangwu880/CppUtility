/*
brief:
有效状态机参考代码
*/

#pragma once
#include "../utility/typedef.h"

class StateObj //比如玩家，AI对象
{
public:
private:

};

class StateMachine;
//状态类尽量别保存对象信息
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

//状态对象简单，可以考虑去掉StateMachine类，由状态对象来管理状态机
class StateMachine
{
public:
	StateMachine();
	void ChangeState(BaseState * m_state);//注意参数只能位本对象的成员之一，别怼到别的对象了
	void Event(); //传递各种事件到具体状态，可以加更多事件接口

public:
	//所有状态列表
	State1 m_state1;
	State2 m_state2;

private:
	BaseState *m_p_cur; //当前状态
	BaseState *m_p_last; //最近历史 状态

};