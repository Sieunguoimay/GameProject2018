#include "Animal.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"
Animal::Animal(const glm::vec2& pos)
	:Animal(glm::vec4(pos.x, pos.y, pos.x, pos.y))
{
}
Animal::Animal(const glm::vec4 & aabb)
	:AnimationBodyEntity(new AnimationSkin(
		Locator::GetAssets()->SpawnSpriterEntity("skin_2/main_skin/entity_000"), 1.0f),
		/*ObjectType::NULL_TYPE,*/ aabb)
{
	m_skin->SetAnimation(m_skin->GetSpriterEntity()->GetAnimationIndex("run"));
}
Animal::~Animal(){}

void Animal::SetupBody(bool atRunTime /*= false*/)
{
	glm::vec2 pos = glm::vec2((m_AABB.z + m_AABB.x) / 2.0f, (m_AABB.w + m_AABB.y) / 2.0f);
	glm::vec2 dimension = glm::vec2(80, 160);

	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_dynamicBody, MEAT, b2Vec2(pos.x/M2P,pos.y/M2P));

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / (2.0f*M2P));
	m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MEAT));

	//sensor shape
	//cs.m_radius = (dimension.x+10.0f) / M2P;
	//cs.m_p.Set(0.0f, 0.0f);
	//m_pBody->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(SENSOR,&cs));

	m_body->SetFixedRotation(true);
	m_body->SetUserData(this);
}


//
//void Animal::OnBeginContact(b2Contact * contact, b2Fixture * fixture)
//{
//}

void Animal::Update(float deltaTime)
{
	//super
	AnimationBodyEntity::Update(deltaTime);
}

Spawner * Animal::Spawn(InfoPacket*info)
{
	//x y z w
	std::string s=*((std::string*)info->GetData());
	auto&ss=split(s, ' ');
	glm::vec4 aabb;

	aabb.x = atof(ss[0].c_str());
	aabb.y = atof(ss[1].c_str());

	if(ss.size()==2){
		Spawner*a = (new Animal(glm::vec2(aabb.x, aabb.y)))->SetId(info->GetId());
		delete info;
		return a;
	}

	aabb.z = atof(ss[2].c_str());
	aabb.w = atof(ss[3].c_str());
	Spawner*a = (new Animal(aabb))->SetId(info->GetId());
	delete info;
	return a;
}






Rock::Rock()
	:NoAnimationBodyEntity(
		new NoAnimationSkin(
			Locator::GetAssets()->GetTexture("tree/deep/controller_button.png"))
		,/*ObjectType::NULL_TYPE,*/
		glm::vec4(100,100,100,100))
{
}

void Rock::SetupBody(bool atRunTime /*= false*/)
{
	glm::vec2 pos = glm::vec2((m_AABB.z + m_AABB.x) / 2.0f, (m_AABB.w + m_AABB.y) / 2.0f);
	glm::vec2 dimension = glm::vec2(m_AABB.z - m_AABB.x, m_AABB.w - m_AABB.y);

	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_staticBody, MEAT, b2Vec2(pos.x / M2P, pos.y / M2P));

	m_body->SetUserData(this);
}



Grass::Grass(const glm::vec2&pos)
	:AnimationNoBodyEntity(new AnimationSkin(
		Locator::GetAssets()->SpawnSpriterEntity("tree/deep/rocktree/entity_000"), 1.0f),
		glm::vec4(pos.x, pos.y, pos.x, pos.y))
{
}

Spawner * Grass::Spawn(InfoPacket * info)
{
	glm::vec2 pos = *(const glm::vec2*)info->GetData();
	Spawner*a=(new Grass(pos))->SetId(info->GetId());
	delete info;
	return a;
}




Leaf::Leaf(const glm::vec2&pos /*= glm::vec2(0, 0)*/)
	:NoAnimationNoBodyEntity(new NoAnimationSkin(
		Locator::GetAssets()->GetTexture("tree/deep/rock.png")),
		pos,glm::vec2(100,100))
{
}

Spawner * Leaf::Spawn(InfoPacket * info)
{
	Spawner*a=(new Leaf(*(const glm::vec2*)info->GetData()))->SetId(info->GetId());
	delete info;
	return a;
}
