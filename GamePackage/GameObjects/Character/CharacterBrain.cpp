#include "CharacterBrain.h"
#include"../../misc/Locator.h"
#include"TerrestrialBody.h"

CharacterBrain::CharacterBrain(BodyBase*body, AnimationSkin*skin)
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

	//m_pSkin->SetScale(0.85f);
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
	TerrestrialBody*body = (TerrestrialBody*)(m_pBodyBase);
	if (body->IsOnGround()) {
		if (s_jump_flag) return;

		if ((m_left|| m_right) &&m_jump) {//run and jump
			
			s_jump_flag = true;

			body->JumpAndRun(m_right);//called again and again
			m_pSkin->SetFlip(m_right? FLIP_NONE : HORIZONTAL_FLIP);

			if (StateMachine<CharacterBrain>::IsInState(m_jumping))
				StateMachine<CharacterBrain>::ChangeState(m_jumpingBuffer);
			else 
				StateMachine<CharacterBrain>::ChangeState(m_jumping);
		}
		else if (m_left || m_right) {//run

			body->Run(m_right);
			m_pSkin->SetFlip(m_left? FLIP_NONE : HORIZONTAL_FLIP);

			StateMachine<CharacterBrain>::ChangeState(m_running);
		}
		else if (m_jump) {

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

#include"../../misc/Assistances.h"
Player::Player() :Character(NULL, glm::vec4()){}

Player::Player(const glm::vec4 & AABB)
	:Character(new AnimationSkin(Locator::GetAssets()->SpawnSpriterEntity("skin_2/main_skin/entity_000"),1.0f),AABB)
{
	m_bodyBase = new TerrestrialBody(&m_body);
	m_brain = new CharacterBrain(m_bodyBase, m_skin);
}

Spawner * Player::Spawn(InfoPacket * info)
{
	std::string s = *((std::string*)info->GetData());
	auto&ss = split(s, ' ');
	glm::vec4 aabb(atof(ss[0].c_str()),atof(ss[1].c_str()),0,0);
	if (ss.size() == 2) {
		aabb.z = aabb.x;
		aabb.w = aabb.y;
	}
	else {
		aabb.z = atof(ss[2].c_str());
		aabb.w = atof(ss[3].c_str());
	}
	Spawner*a=(new Player(aabb))->SetId(info->GetId());
	delete info;
	return a;
}

void Player::SetupBody(bool atRunTime)
{
	glm::vec2 pos = glm::vec2((m_AABB.z + m_AABB.x) / 2.0f, (m_AABB.w + m_AABB.y) / 2.0f);
	((TerrestrialBody*)m_bodyBase)->SetupBody(pos, glm::vec2(80, 160));
}
