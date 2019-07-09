#pragma once
#include"../../../misc/StateMachine/StateMachine.h"
#include"../../SpriterEntity.h"
class Player;
class BaseCharacterState :public State<Player> {
protected:
	int m_animationIndex;
public:
	virtual void Enter(Player*entity) override;
	virtual void Execute(Player*entity, float deltaTime) override {}
	virtual void Exit(Player*entity) override {}

	void SetAnimationIndex(int index) { m_animationIndex = index; }
};

class _StandingState :public BaseCharacterState {
public:
	//Enter() methods just for log
	void Enter(Player*entity) override;
};

class _RunningState :public BaseCharacterState {
	float m_pelvisAngle;
	float calculatePelvisAngle(float normalAngle, bool flip);
public:

	void Enter(Player*entity) override;
	void Execute(Player*entity, float deltaTime) override;
	void Exit(Player*entity)override;
};

class _JumpingState :public BaseCharacterState {
public:
	void Enter(Player*entity) override;
	void Execute(Player*entity, float deltaTime) override;
};
class _FallingState :public BaseCharacterState {
public:
	void Enter(Player*entity) override;
};

class _JumpingBufferState :public BaseCharacterState {
public:
	void Enter(Player*entity) override;
	void Execute(Player*entity, float deltaTime) override;
};

class _StoppingState :public BaseCharacterState {
public:
	void Enter(Player*entity) override;
	void Execute(Player*entity, float deltaTime) override;
};


class _GlobalCharacterState :public BaseCharacterState {
public:
	void Enter(Player*entity) override;
	void Execute(Player*entity, float deltaTime) override;

};
