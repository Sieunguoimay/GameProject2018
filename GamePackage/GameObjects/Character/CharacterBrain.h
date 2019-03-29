#pragma once
#include"Character.h"
#include"../../misc/StateMachine/StateMachine.h"
#include"CharacterStates.h"


class CharacterBrain :public Brain,public StateMachine<CharacterBrain> {
	BaseCharacterState*m_standing;
	BaseCharacterState*m_running;
	BaseCharacterState*m_jumping;
	BaseCharacterState*m_falling;
	BaseCharacterState*m_jumpingBuffer;
	BaseCharacterState*m_stopping;

	_GlobalCharacterState*m_globalState;

	friend class _StoppingState;
	friend class _JumpingBufferState;
public:
	CharacterBrain(BodyBase*body, AnimationSkin*skin);
	~CharacterBrain();
	void Update(float deltaTime);

	//getters
	inline BaseCharacterState*GetFallingState()const { return m_falling; }

private:
	//control
	bool m_jump;
	bool m_right;
	bool m_left;
};

class Player :public Character{
public:
	Player();
	Player(const glm::vec4&AABB);
	Spawner*Spawn(InfoPacket*info)override;
	void SetupBody(bool atRunTime=false)override;
};
