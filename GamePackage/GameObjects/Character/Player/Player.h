#pragma once
#include"../../EntityHierachy/HavingBodyEntity.h"
#include"../../../misc/StateMachine/StateMachine.h"
#include"CharacterStates.h"
#include"TerrestrialBody.h"
#include"PlayerSkin.h"
class Player :public AnimationBodyEntity, public StateMachine<Player> {
	BaseCharacterState*m_standing;
	BaseCharacterState*m_running;
	BaseCharacterState*m_jumping;
	BaseCharacterState*m_falling;
	BaseCharacterState*m_jumpingBuffer;
	BaseCharacterState*m_stopping;

	_GlobalCharacterState*m_globalState;

	friend class _StoppingState;
	friend class _JumpingBufferState;
	friend class _JumpingState;
	friend class _GlobalCharacterState;
	friend class BaseCharacterState;
	//control
	bool m_jump;
	bool m_right;
	bool m_left;
	bool m_holeObject;

	TerrestrialBody*m_terrestrialBody;

public:
	Player():AnimationBodyEntity(NULL,/*ObjectType::NULL_TYPE,*/glm::vec4(0.0f)){}
	Player(const glm::vec4&AABB);
	~Player();

	//seting functions
	Spawner*Spawn(InfoPacket*info)override;
	void SetupBody(bool atRunTime=false)override;
	void Update(float deltaTime);
	inline TerrestrialBody*GetTerrestrialBody() { return m_terrestrialBody; }

	//What I can do
	inline void JumpSignal() { m_jump = true; }
	inline void RightSignal() { m_right = true; }
	inline void LeftSignal() { m_left = true;}
	inline void HoldIntendedObject() { m_holeObject = true; }
	
};

