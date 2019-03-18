#include "HavingBodyEntity.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"

HavingBodyEntity::HavingBodyEntity(Skin * skin, ObjectType type)
	:BodyProtocol(type), m_skinBase(skin), m_body(NULL)
{

}

HavingBodyEntity::~HavingBodyEntity()
{
	Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
	delete m_skinBase;
}

void HavingBodyEntity::OnSelect(const glm::vec2 & AA, const glm::vec2 & BB)
{
}

void HavingBodyEntity::OnRelease(const glm::vec2 & AA, const glm::vec2 & BB)
{
}

void HavingBodyEntity::Update(float deltaTime)
{
	//super
	AABBEntity::Update(deltaTime);

	b2Vec2 pos = M2P*m_body->GetPosition();
	m_skinBase->SetPos(glm::vec2(pos.x, pos.y));
	m_skinBase->SetAngle(m_body->GetAngle());

}

void HavingBodyEntity::Draw()
{
	//super
	AABBEntity::Draw();

	m_skinBase->Draw();
}








AnimationBodyEntity::AnimationBodyEntity(AnimationSkin * skin, ObjectType type)
	:HavingBodyEntity(skin,type)
{
	m_skin = (AnimationSkin*)m_skinBase;
}

AnimationBodyEntity::~AnimationBodyEntity()
{
}

void AnimationBodyEntity::Update(float deltaTime)
{
	//super
	HavingBodyEntity::Update(deltaTime);
	m_skin->Update(deltaTime);
}




NoAnimationBodyEntity::NoAnimationBodyEntity(NoAnimationSkin * skin, ObjectType type)
	:HavingBodyEntity(skin, type)
{
	m_skin = (NoAnimationSkin*)m_skinBase;
}

NoAnimationBodyEntity::~NoAnimationBodyEntity()
{
}

