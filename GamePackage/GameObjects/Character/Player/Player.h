#pragma once
#include"../../EntityHierachy/HavingBodyEntity.h"
#include"../../../misc/StateMachine/StateMachine.h"
#include"ActionStates.h"
#include"PlayerBody.h"
#include"PlayerSkin.h"
#include"BrainStates.h"



class Player :public AnimationBodyEntity, public StateMachine<Player>,public PlayerBodyCallback {
	//control

	bool m_jump;
	bool m_right;
	bool m_left;
	bool m_holdTouchPointNow;

	PlayerBody*m_playerBody;

	
	BrainState* m_brainStates[BrainStateEnum::BS_TOTAL_NUM];

public:
	Player():AnimationBodyEntity(NULL,/*ObjectType::NULL_TYPE,*/glm::vec4(0.0f)){
		SDL_Log("Created player prototype");
	}
	Player(const glm::vec4&AABB);
	~Player();

	//seting functions
	Spawner*Spawn(InfoPacket*info)override;
	void SetupBody(bool atRunTime=false)override;
	void Update(float deltaTime)override;

	//callback implementation
	inline void OnStateChanged(int brainState)override { ChangeBrainState(brainState); }


	inline PlayerBody*GetPlayerBody() { return m_playerBody; }
	inline void ChangeBrainState(int state) {ChangeState(m_brainStates[state]);}

	//What I can do
	inline void JumpSignal() { m_jump = true;}
	inline void RightSignal() { m_right = true; }
	inline void LeftSignal() { m_left = true;}
	inline void HoldTouchPoint() { m_holdTouchPointNow = true; }
	

	friend class BrainStateOnTheFloor;
	friend class BrainStateNextToRockLHS;
	friend class BrainStateNextToRockRHS;
	friend class BrainStateOnRockTopRight;
	friend class BrainStateOnRockTopLeft;
	friend class BrainStateTest;
};

