#pragma once
#include"../../../misc/StateMachine/StateMachine.h"
#include<iostream>
#include<queue>
#include"../../PhysicsEngine/PrimitiveActions.h"
enum BrainStateEnum {
	BS_ON_THE_FLOOR,
	BS_NEXT_TO_ROCK_LHS,
	BS_NEXT_TO_ROCK_RHS,
	BS_ON_ROCK_TOP_RIGHT,
	BS_ON_ROCK_TOP_LEFT,
	BS_LONG_ACTION,
	BS_TOTAL_NUM
};

class BrainState: public State<class Player>{
protected:
	Player*d;//d stands for Duong :)
public:
	inline void Enter(Player*entity) override { d = entity; Enter(); }
	inline void Execute(Player*entity, float deltaTime) override { Execute(deltaTime); }
	inline void Exit(Player*entity) override { Exit(); }

	virtual void Enter() = 0;
	virtual void Execute(float deltaTime) = 0;
	virtual void Exit() = 0;
};

class BrainStateOnTheFloor :public BrainState {
public:
	virtual void Enter() override;
	virtual void Execute(float deltaTime) override;
	virtual void Exit() override;
};

class BrainStateNextToRockLHS:public BrainState {
public:
	virtual void Enter() override;
	virtual void Execute(float deltaTime) override;
	virtual void Exit() override;
};

class BrainStateNextToRockRHS :public BrainState {
public:
	virtual void Enter() override;
	virtual void Execute(float deltaTime) override;
	virtual void Exit() override;
};

class BrainStateOnRockTopRight:public BrainState {
public:
	virtual void Enter() override;
	virtual void Execute(float deltaTime) override;
	virtual void Exit() override;
};

class BrainStateOnRockTopLeft:public BrainState {
public:
	virtual void Enter() override;
	virtual void Execute(float deltaTime) override;
	virtual void Exit() override;
};




#include"PlayerActuator.h"
class BrainStateLongAction :public BrainState {
	State<Player>*m_followState = NULL;
	std::queue<pa::PlayerLongActionBase*>m_actions;
	bool m_enterFlag;
public:
	~BrainStateLongAction()override;
	void SetFollowState(State<Player>*followState) { m_followState = followState; }
	void AddAction(pa::PlayerLongActionBase *action) { m_actions.push(action); }
	virtual void Enter() override;
	virtual void Execute(float deltaTime) override;
	virtual void Exit() override;
};