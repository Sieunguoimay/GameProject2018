#include "CharacterBrain.h"
#include"../../misc/Locator.h"
#include"TerrestrialBody.h"

CharacterBrain::CharacterBrain(BodyBase*body, Skin*skin)
	:Brain(body,skin)
{
	int runIndex = m_pSkin->GetSpriterEntity()->GetAnimationIndex("run");
	Timeline*pelvisBone = m_pSkin->GetSpriterEntity()->GetTimeline(runIndex,"bone_005");
	m_globalState = new _GlobalCharacterState(
		&m_pSkin->GetSpriterEntity()->GetSclmEntity()->animations,
		m_pSkin->GetSpriterEntity()->GetSclmEntity()->animationMap.size(),
		m_pSkin->GetSpriterEntity()->GetDefaultTimelineIndex("bone_012"));

	(m_running = new _RunningState(pelvisBone))->
		SetAnimationIndex(runIndex);
	(m_jumping = new _JumpingState())->
		SetAnimationIndex(m_pSkin->GetSpriterEntity()->GetAnimationIndex("jump"));
	(m_falling = new _FallingState())->
		SetAnimationIndex(m_pSkin->GetSpriterEntity()->GetAnimationIndex("fall"));
	(m_standing = new _StandingState())->
		SetAnimationIndex(m_pSkin->GetSpriterEntity()->GetAnimationIndex("stand"));
	(m_stopping = new _StoppingState())->
		SetAnimationIndex(m_pSkin->GetSpriterEntity()->GetAnimationIndex("stop"));
	(m_jumpingBuffer = new _JumpingBufferState())->
		SetAnimationIndex(m_pSkin->GetSpriterEntity()->GetAnimationIndex("jump2"));

	StateMachine<CharacterBrain>::Init(this,m_standing, NULL, m_globalState);

	m_pSkin->SetAnimationSwitchingTime("stand", "run", 200);
	m_pSkin->SetAnimationSwitchingTime("stand", "jump", 180);
	m_pSkin->SetAnimationSwitchingTime("run", "stand", 200);
	m_pSkin->SetAnimationSwitchingTime("run", "jump", 180);
	m_pSkin->SetAnimationSwitchingTime("run", "stop", 200);
	m_pSkin->SetAnimationSwitchingTime("jump", "run", 300);
	m_pSkin->SetAnimationSwitchingTime("jump", "jump2", 20);
	m_pSkin->SetAnimationSwitchingTime("jump2", "jump", 20);
	m_pSkin->SetAnimationSwitchingTime("stop", "stand", 300);

	m_pSkin->SetScale(0.85f);
}

CharacterBrain::~CharacterBrain()
{
	delete m_running;
	delete m_jumping;
	delete m_falling;
	delete m_standing;
	delete m_stopping;
	delete m_jumpingBuffer;
	delete m_globalState;
}

void CharacterBrain::Update(float deltaTime)
{

	StateMachine<CharacterBrain>::Update(deltaTime);

	//when to do what logics
	//to deal with the 2 loops in one jump
	static bool s_jump_flag = false;
	TerrestrialBody*body = (TerrestrialBody*)(m_pBody);
	if (body->IsOnGround()) {
		if (s_jump_flag) return;

		if ((
			s_controller->Left() || 
			s_controller->Right()) &&
			s_controller->Up()) {//run and jump
			
			s_jump_flag = true;

			body->JumpAndRun(s_controller->Right());//called again and again
			m_pSkin->SetFlip(s_controller->Right() ? FLIP_NONE : HORIZONTAL_FLIP);

			if (StateMachine<CharacterBrain>::IsInState(m_jumping))
				StateMachine<CharacterBrain>::ChangeState(m_jumpingBuffer);
			else 
				StateMachine<CharacterBrain>::ChangeState(m_jumping);
		}
		else if (
			s_controller->Left() || 
			s_controller->Right()) {//run

			body->Run(s_controller->Right());
			m_pSkin->SetFlip(s_controller->Right() ? FLIP_NONE : HORIZONTAL_FLIP);

			StateMachine<CharacterBrain>::ChangeState(m_running);
		}
		else if (s_controller->Up()) {

			s_jump_flag = true;

			body->Jump();

			if (StateMachine<CharacterBrain>::IsInState(m_jumping))
				StateMachine<CharacterBrain>::ChangeState(m_jumpingBuffer);
			else 
				StateMachine<CharacterBrain>::ChangeState(m_jumping);
			
		}
		else {//stand

			body->Stop();

			if(StateMachine<CharacterBrain>::IsInState(m_running)|| 
				StateMachine<CharacterBrain>::IsInState(m_stopping))
				StateMachine<CharacterBrain>::ChangeState(m_stopping);
			else 
				StateMachine<CharacterBrain>::ChangeState(m_standing);
			
		}
	}
	else {//on air
		s_jump_flag = false;
	}
}
