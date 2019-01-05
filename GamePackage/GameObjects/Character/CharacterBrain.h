#pragma once
#include"Character.h"
#include"../../misc/StateMachine/StateMachine.h"
class CharacterBrain;
class _StandingState:public State<CharacterBrain>{
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
	void Exit(CharacterBrain*entity) override;
};
class _RunningState :public State<CharacterBrain> {
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
	void Exit(CharacterBrain*entity) override;
};
class _JumpingState :public State<CharacterBrain> {
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
	void Exit(CharacterBrain*entity) override;
};
class _JumpingBufferState :public State<CharacterBrain> {
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
	void Exit(CharacterBrain*entity) override;
};
class _StoppingState :public State<CharacterBrain> {
public:
	void Enter(CharacterBrain*entity) override;
	void Execute(CharacterBrain*entity, float deltaTime) override;
	void Exit(CharacterBrain*entity) override;
};





class CharacterBrain :public Brain,public StateMachine<CharacterBrain> {
	State<CharacterBrain>*m_standing;
	State<CharacterBrain>*m_running;
	State<CharacterBrain>*m_jumping;
	State<CharacterBrain>*m_jumpingBuffer;
	State<CharacterBrain>*m_stopping;
	friend class _StoppingState;
	friend class _JumpingBufferState;
public:
	CharacterBrain(BodyBase*body, Skin*skin);
	~CharacterBrain();
	void Update(float deltaTime);

};
