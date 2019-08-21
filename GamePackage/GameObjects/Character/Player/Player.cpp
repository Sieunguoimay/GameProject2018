#include "Player.h"
#include"../../../misc/Locator.h"
#include"../../../misc/Assistances.h"

Player::Player(const glm::vec4 & AABB)
	:AnimationBodyEntity(new PlayerSkin(
		Locator::GetAssets()->SpawnSpriterEntity("skin_2/main_skin/entity_000"),1.0f),
		/*ObjectType::PLAYER, */AABB)
{
	m_playerBody = new PlayerBody((PlayerSkin*)m_skin,this);

	m_brainStates[BS_ON_THE_FLOOR] = new BrainStateOnTheFloor();
	m_brainStates[BS_NEXT_TO_ROCK_LHS] = new BrainStateNextToRockLHS();
	m_brainStates[BS_NEXT_TO_ROCK_RHS] = new BrainStateNextToRockRHS();
	m_brainStates[BS_ON_ROCK_TOP_LEFT] = new BrainStateOnRockTopLeft();
	m_brainStates[BS_ON_ROCK_TOP_RIGHT] = new BrainStateOnRockTopRight();
	m_brainStates[BS_TEST_STATE] = new BrainStateTest();
	StateMachine<Player>::Init(this, m_brainStates[BS_ON_THE_FLOOR], NULL, NULL);

	SDL_Log("Created player");
}


void Player::SetupBody(bool atRunTime)
{
	glm::vec2 pos = glm::vec2((m_AABB.z + m_AABB.x) / 2.0f, (m_AABB.w + m_AABB.y) / 2.0f);
	m_body = m_playerBody->SetupBody(pos, glm::vec2(80, 160));
}


Player::~Player() {

	for (int i = 0; i < BS_TOTAL_NUM; i++)
		delete m_brainStates[i];

	delete m_playerBody;
	SDL_Log("Deleted Player");
}




void Player::Update(float deltaTime) {

		//if ((m_left || m_right) && m_jump) {//run and jump

		//	m_playerBody->_JumpAndRun(m_right);//called again and again
		//	m_skin->SetFlip(m_right ? FLIP_NONE : HORIZONTAL_FLIP);

		//	//StateMachine<Player>::ChangeState(StateMachine<Player>::IsInState(m_jumping) ? m_jumpingBuffer : m_jumping);
		//}

		//if (m_jump) {
		//	m_playerBody->_Jump();
		//	//StateMachine<Player>::ChangeState(StateMachine<Player>::IsInState(m_jumping) ? m_jumpingBuffer : m_jumping);
		//}
		//if (StateMachine<Player>::IsInState(m_falling)) {
		//	StateMachine<Player>::ChangeState(m_standing);
		//}

		//if (StateMachine<Player>::IsInStates(m_standing,m_running,NULL)) {
		//	if (m_left || m_right) {//run

		//		if (m_holdTouchPointNow) {
		//			m_playerBody->_Move(m_right, WALK_SPEED);
		//			//walking forward or backward depends on the direction of the body and the relative position of 
		//			// the rock with respect to the body. 
		//			//if on the same direction then forward
		//			//els backward
		//			StateMachine<Player>::ChangeState(m_walkingBackward);
		//		}
		//		else {
		//			m_skin->SetFlip(m_right ? FLIP_NONE : HORIZONTAL_FLIP);
		//			StateMachine<Player>::ChangeState(m_running);
		//			m_playerBody->_Move(m_right, RUN_SPEED);
		//		}
		//	}
		//}
		//}
		//else {//stand

		//	m_playerBody->Stop();

		//	if (StateMachine<Player>::IsInState(m_running) ||
		//		StateMachine<Player>::IsInState(m_stopping))
		//		StateMachine<Player>::ChangeState(m_stopping);
		//	else
		//		StateMachine<Player>::ChangeState(m_standing);

		//}

		//while standing on ground we can hold object??
		//if (m_holdTouchPointNow && !m_jump) {
		//	m_playerBody->HoldTouchPoint(1.0f / M2P * Utils::glmToB2(((PlayerSkin*)m_skin)->GetArmBone1()->GetPos()));
		//}
		//else
		//	m_playerBody->LeaveTouchPoint();




	StateMachine<Player>::Update(deltaTime);
	m_playerBody->Update(deltaTime);
	AnimationBodyEntity::Update(deltaTime);

	m_jump = false;
	m_right = false;
	m_left = false;
	m_holdTouchPointNow = false;
}




Spawner * Player::Spawn(InfoPacket * info)
{
	std::string s = *((std::string*)info->GetData());
	auto&ss = Utils::split(s, ' ');
	glm::vec4 aabb(atof(ss[0].c_str()),atof(ss[1].c_str()),0,0);
	if (ss.size() == 2){
		aabb.z = aabb.x;
		aabb.w = aabb.y;
	}
	else {
		aabb.z = Utils::_atof(ss[2].c_str());
		aabb.w = Utils::_atof(ss[3].c_str());
	}
	Spawner*a=(new Player(aabb))->SetId(info->GetId());
	delete info;
	return a;
}
