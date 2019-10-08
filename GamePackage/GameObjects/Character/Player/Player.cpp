#include "Player.h"
#include"../../../misc/Locator.h"
#include"../../../misc/Assistances.h"

Player::Player(const glm::vec4 & AABB)
	: AnimationBodyEntity(Locator::GetAssets()->SpawnSpriterEntity("skin_2/main_skin/entity_000"),AABB)
	, m_decisionMaker(this)
	, m_actuator(this)
	, m_sensor(this)
	, m_memory(this)
	, m_skin(this)
{
	SDL_Log("Created player");
}


void Player::SetupBody(bool atRunTime)
{
	glm::vec2 pos = glm::vec2((m_AABB.z + m_AABB.x) / 2.0f, (m_AABB.w + m_AABB.y) / 2.0f);
	glm::vec2 dimension(80, 160);

	//Create the circles
	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_dynamicBody, MEAT, b2Vec2(pos.x / M2P, pos.y / M2P));

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / (2.0f*M2P));
	m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MEAT));

	//sensor shape

	//cs.m_radius = (dimension.x+10.0f) / M2P;
	//cs.m_p.Set(0.0f, 0.0f);
	//m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(SENSOR,&cs));

	m_body->SetFixedRotation(true);
	m_body->SetUserData(m_bodyBaseThis);
	m_bodyId = BodyObjectType::BOT_PLAYER;
	//m_leg.Init(body);

	SDL_Log("Player's Mass: %f", m_body->GetMass());
}


Player::~Player() {
	SDL_Log("Deleted Player");
}




void Player::Update(float deltaTime) {

	m_control.Update(deltaTime);
	m_sensor.Update(deltaTime);
	m_decisionMaker.Update(deltaTime);
	m_actuator.Update(deltaTime);
	m_skin.Update(deltaTime);
	AnimationBodyEntity::Update(deltaTime);
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
