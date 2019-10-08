#pragma once
#include"../../../misc/StateMachine/StateMachine.h"
#include"../../SpriterEntity.h"

enum ActionStateEnum {
	AS_RUN_LEFT			,
	AS_RUN_RIGHT		,
	AS_JUMP				,
	AS_FALL				,
	AS_JUMP_LEFT		,
	AS_JUMP_RIGHT		,
	AS_PUSH_LEFT		,
	AS_PUSH_RIGHT		,
	AS_PULL_LEFT		,
	AS_PULL_RIGHT		,
	AS_CLIMB_ON_LEFT	,
	AS_CLIMB_ON_RIGHT	,
	AS_CLIMB_DOWN_LEFT	,
	AS_CLIMB_DOWN_RIGHT	,
	AS_STOP				,
	AS_STAND			,
	AS_TOTAL_NUM		
};

class BaseActionState :public State<class PlayerActuator> {
	bool m_setAnimationOnEnterFlag;
	friend class PlayerActuator;
protected:
	int m_flipFlag = -1;
	int m_animationIndex;
	std::string m_name = "No name";
	PlayerActuator*m_pPlayerActuator;

public:
	BaseActionState(int index, int flip = -1):m_animationIndex(index), m_flipFlag(flip), m_setAnimationOnEnterFlag(true){}
	BaseActionState() { SDL_Log("Created BaseCharacterState"); }
	~BaseActionState() { SDL_Log("Destroyed BaseCharacterState"); }

	virtual void Enter(PlayerActuator*entity) override;
	virtual void Execute(PlayerActuator*entity, float deltaTime) override { Execute(); }
	virtual void Exit(PlayerActuator*entity) override { Exit(); }

	BaseActionState* SetAnimationIndex(int index) { m_animationIndex = index; return this; }
	BaseActionState* SetName(const std::string&name) { m_name = name; return this; }
	BaseActionState* SetFlipFlag(int flipFlag) { m_flipFlag = flipFlag; return this;}
	virtual int GetAnimationIndex() const { return m_animationIndex; }

	virtual void Enter(){}
	virtual void Execute(){}
	virtual void Exit(){}

};



class _JumpingState :public BaseActionState {
public:
	void Execute() override;
};

class _JumpingBufferState :public BaseActionState {
public:
	void Execute() override;
};

class _StoppingState :public BaseActionState {
public:
	void Execute() override;
};

class _ClimbingState:public BaseActionState {
	BaseActionState*m_nextState;
public:
	_ClimbingState(int preIndex, int index, int flip);
	~_ClimbingState();
	void Execute()override;
};

