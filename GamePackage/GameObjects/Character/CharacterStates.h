#pragma once
#include"../../misc/StateMachine/StateMachine.h"
#include"../SpriterEntity.h"
class CharacterBrain;
class BaseCharacterState :public State<CharacterBrain> {
protected:
	int m_animationIndex;
public:
	virtual void Enter(CharacterBrain*entity) override;
	virtual void Execute(CharacterBrain*entity, float deltaTime) override {}
	virtual void Exit(CharacterBrain*entity) override {}

	void SetAnimationIndex(int index) { m_animationIndex = index; }
};

class _StandingState :public BaseCharacterState {
public:
	//Enter() methods just for log
	void Enter(CharacterBrain*entity) override;
};

class _RunningState :public BaseCharacterState {
	Timeline*m_pPelvisBone;
	float m_pelvisAngle;
	float calculatePelvisAngle(float normalAngle, bool flip);
public:
	_RunningState(Timeline*pPelvisBone);

	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
};

class _JumpingState :public BaseCharacterState {
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
};
class _FallingState :public BaseCharacterState {
public:
	void Enter(CharacterBrain*entity) override;
};

class _JumpingBufferState :public BaseCharacterState {
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
};

class _StoppingState :public BaseCharacterState {
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
};


class _GlobalCharacterState :public BaseCharacterState {
	enum BoneName {
		LEFT_HAND,
		NUM_CONTROLED_BONES
	};
	std::vector<Timeline*>m_leftHandBone;
	bool m_flag=false;
public:
	_GlobalCharacterState(std::vector<AnimationBase*>*animations,int size, int timelineIndex);
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
};