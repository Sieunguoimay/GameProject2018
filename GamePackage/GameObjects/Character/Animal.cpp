#include "Animal.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"
Animal::Animal(glm::vec2 pos)
	:AnimationBodyEntity(new AnimationSkin(
		Locator::GetAssets()->SpawnSpriterEntity("skin_2/main_skin/entity_000"), 1.0f),
		ObjectType::NULL_TYPE)
	,m_pos(pos)
{
	m_skin->SetAnimation(m_skin->GetSpriterEntity()->GetAnimationIndex("run"));
	SetupBody();
}
Animal::~Animal(){}

void Animal::SetupBody()
{
	glm::vec2 dimension = glm::vec2(80, 160);

	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_dynamicBody, MEAT, b2Vec2(m_pos.x/M2P,m_pos.y/M2P));

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / (2.0f*M2P));
	m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MEAT));

	//sensor shape
	//cs.m_radius = (dimension.x+10.0f) / M2P;
	//cs.m_p.Set(0.0f, 0.0f);
	//m_pBody->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(SENSOR,&cs));

	m_body->SetFixedRotation(true);
	m_body->SetUserData(this);
}



void Animal::OnBeginContact(b2Contact * contact, b2Fixture * fixture)
{
}

void Animal::Update(float deltaTime)
{
	//super
	AnimationBodyEntity::Update(deltaTime);
}

Rock::Rock()
	:NoAnimationBodyEntity(new NoAnimationSkin(Locator::GetAssets()->GetTexture("tree/deep/controller_button.png")),ObjectType::NULL_TYPE)
{
	SetupBody();
}

void Rock::SetupBody()
{
	glm::vec2 dimension = glm::vec2(80, 160);

	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_dynamicBody, MEAT, b2Vec2(1, 0));

	m_body->SetUserData(this);
}

Grass::Grass()
	:AnimationNoBodyEntity(new AnimationSkin(
		Locator::GetAssets()->SpawnSpriterEntity("tree/deep/rocktree/entity_000"), 1.0f),
		glm::vec2(0,0))
{
	m_skin->SetAnimation(m_skin->GetSpriterEntity()->GetAnimationIndex("standing"));
}

Leaf::Leaf()
	:NoAnimationNoBodyEntity(new NoAnimationSkin(
		Locator::GetAssets()->GetTexture("tree/deep/rock.png")),
		glm::vec2(-100,300),glm::vec2(100,100))
{
}

FlockOfAnimal::FlockOfAnimal()
{
	m_entities.push_back(new Animal(glm::vec2(100, 0)));
	m_entities.push_back(new Animal(glm::vec2(200,100)));
}
