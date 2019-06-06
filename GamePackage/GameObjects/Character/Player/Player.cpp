#include "Player.h"
#include"../../../misc/Locator.h"
#include"../../../misc/Assistances.h"

Player::Player(const glm::vec4 & AABB)
	:AnimationBodyEntity(new PlayerSkin(
		Locator::GetAssets()->SpawnSpriterEntity("skin_2/main_skin/entity_000"),1.0f),
		/*ObjectType::PLAYER, */AABB)
{
	m_terrestrialBody = new TerrestrialBody();
	m_body = m_terrestrialBody->GetBody();

	m_globalState = new _GlobalCharacterState(
		m_skin->GetSpriterEntity()->GetSegmentIndex("bone_012"));

	(m_running = new _RunningState(
		m_skin->GetSpriterEntity()->GetSegmentIndex("bone_005"),this))->
		SetAnimationIndex(m_skin->GetSpriterEntity()->GetAnimationIndex("run"));

	(m_jumping = new _JumpingState())->
		SetAnimationIndex(m_skin->GetSpriterEntity()->GetAnimationIndex("jump"));
	(m_falling = new _FallingState())->
		SetAnimationIndex(m_skin->GetSpriterEntity()->GetAnimationIndex("fall"));
	(m_standing = new _StandingState())->
		SetAnimationIndex(m_skin->GetSpriterEntity()->GetAnimationIndex("stand"));
	(m_stopping = new _StoppingState())->
		SetAnimationIndex(m_skin->GetSpriterEntity()->GetAnimationIndex("stop"));
	(m_jumpingBuffer = new _JumpingBufferState())->
		SetAnimationIndex(m_skin->GetSpriterEntity()->GetAnimationIndex("jump2"));

	StateMachine<Player>::Init(this, m_standing, NULL, m_globalState);

}


void Player::SetupBody(bool atRunTime)
{
	glm::vec2 pos = glm::vec2((m_AABB.z + m_AABB.x) / 2.0f, (m_AABB.w + m_AABB.y) / 2.0f);
	m_terrestrialBody->SetupBody(pos, glm::vec2(80, 160), m_body);
}


Player::~Player() {
	delete m_running;
	delete m_jumping;
	delete m_falling;
	delete m_standing;
	delete m_stopping;
	delete m_jumpingBuffer;
	delete m_globalState;

	delete m_terrestrialBody;
}




void Player::Update(float deltaTime) {


	StateMachine<Player>::Update(deltaTime);

	//when to do what logics
	//to deal with the 2 loops in one jump
	static bool s_jump_flag = false;
	bool flag = false;
	if (m_terrestrialBody->IsOnGround()) {
		if (s_jump_flag) flag = true;
		if (!flag) {
			if ((m_left || m_right) && m_jump) {//run and jump
				s_jump_flag = true;

				m_terrestrialBody->JumpAndRun(m_right);//called again and again
				m_skin->SetFlip(m_right ? FLIP_NONE : HORIZONTAL_FLIP);

				if (StateMachine<Player>::IsInState(m_jumping))
					StateMachine<Player>::ChangeState(m_jumpingBuffer);
				else
					StateMachine<Player>::ChangeState(m_jumping);
			}
			else if (m_left || m_right) {//run

				m_terrestrialBody->Run(m_right);
				m_skin->SetFlip(m_right ? FLIP_NONE : HORIZONTAL_FLIP);

				StateMachine<Player>::ChangeState(m_running);
			}
			else if (m_jump) {

				s_jump_flag = true;

				m_terrestrialBody->Jump();

				if (StateMachine<Player>::IsInState(m_jumping))
					StateMachine<Player>::ChangeState(m_jumpingBuffer);
				else
					StateMachine<Player>::ChangeState(m_jumping);

			}
			else {//stand

				m_terrestrialBody->Stop();

				if (StateMachine<Player>::IsInState(m_running) ||
					StateMachine<Player>::IsInState(m_stopping))
					StateMachine<Player>::ChangeState(m_stopping);
				else
					StateMachine<Player>::ChangeState(m_standing);

			}
		}
	}
	else if (!flag) {//on air
		s_jump_flag = false;
	}

	//reset all triggers
	if (!flag){
		m_jump = false;
		m_right = false;
		m_left = false;
		m_holeObject = false;
	}


	m_terrestrialBody->Update(deltaTime);
	AnimationBodyEntity::Update(deltaTime);
	//Character::Update(deltaTime);

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
