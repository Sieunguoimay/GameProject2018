#include "HavingBodyEntity.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"

HavingBodyEntity::HavingBodyEntity(Skin * skin, ObjectType type)
	:BodyProtocol(type)
	, BodyDragAndDrop(Locator::GetPhysicsFactory()->GetB2World())
	,m_skinBase(skin), m_body(NULL)
{
}

HavingBodyEntity::~HavingBodyEntity()
{
	if(m_body) Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
	if(m_skinBase) delete m_skinBase;
}

void HavingBodyEntity::OnSelect(const glm::vec4 & AABB)
{
	if(m_body->GetType()!=b2_staticBody)
		DragStart(m_body, b2Vec2((AABB.x + AABB.z) / (2.0f*M2P), (AABB.y + AABB.w) / (2.0f*M2P)));
}

void HavingBodyEntity::OnRelease(const glm::vec4 & AABB)
{
	if (m_body->GetType() != b2_staticBody) 
		Drop();

	else{//in case of static body
		Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
		SetupBody(true);
	}
}

void HavingBodyEntity::ApplyAABB(const glm::vec4 & AABB)
{
	if (m_body->GetType() != b2_staticBody)
		SetTarget(b2Vec2((AABB.x + AABB.z) / (2.0f*M2P), (AABB.y + AABB.w) / (2.0f*M2P)));
}

void HavingBodyEntity::Update(float deltaTime)
{
	//super
	AABBEntity::Update(deltaTime);
	if (AABBEntity::IsSelected())return;

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








AnimationBodyEntity::AnimationBodyEntity(AnimationSkin * skin, ObjectType type, glm::vec4 AABB)
	:HavingBodyEntity(skin,type)
{
	if (m_skinBase == NULL) return;

	m_skin = (AnimationSkin*)m_skinBase;

	if (AABB.z <= AABB.x) {
		const glm::vec4&aabb = m_skin->GetAABB();
		AABB.z = (aabb.z - aabb.x) + AABB.x;
		AABB.w = (aabb.w - aabb.y) + AABB.y;
	}
	m_skin->SetAABB(AABB);
	m_AABB = AABB;
}

AnimationBodyEntity::~AnimationBodyEntity()
{
}

void AnimationBodyEntity::Update(float deltaTime)
{
	//super
	HavingBodyEntity::Update(deltaTime);
	if (AABBEntity::IsSelected())return;

	m_skin->Update(deltaTime);
}

void AnimationBodyEntity::OnSelect(const glm::vec4 & AABB)
{
	HavingBodyEntity::OnSelect(AABB);
	m_animationIndexBuffer = m_skin->GetSpriterEntity()->GetCurrentAnimationIndex();
	m_skin->GetSpriterEntity()->SetAnimationStrictly(0);
}

void AnimationBodyEntity::OnRelease(const glm::vec4 & AABB)
{
	HavingBodyEntity::OnRelease(AABB);
	m_skin->GetSpriterEntity()->SetAnimationStrictly(m_animationIndexBuffer);
}

void AnimationBodyEntity::ApplyAABB(const glm::vec4 & AABB)
{
	HavingBodyEntity::ApplyAABB(AABB);
	m_skin->SetAABB(AABB);//-> not support zooming with AnimationBodyEntity
}




NoAnimationBodyEntity::NoAnimationBodyEntity(NoAnimationSkin * skin, ObjectType type, glm::vec4 AABB)
	:HavingBodyEntity(skin, type)
{
	if (m_skinBase == NULL) return;

	m_skin = (NoAnimationSkin*)m_skinBase;

	if (AABB.z <= AABB.x) {
		AABB.z = m_skin->GetSize().x + AABB.x;
		AABB.w = m_skin->GetSize().y + AABB.y;
	}
	else {
		m_skin->SetSize(glm::vec2(AABB.z - AABB.x, AABB.w - AABB.y));
	}

	m_skin->SetPos(glm::vec2((AABB.x + AABB.z) / 2.0f, (AABB.y + AABB.w) / 2.0f));

	m_AABB = AABB;
}

NoAnimationBodyEntity::~NoAnimationBodyEntity()
{
}

void NoAnimationBodyEntity::OnSelect(const glm::vec4 & AABB)
{
	HavingBodyEntity::OnSelect(AABB);
	m_AABBDimensionBuffer = glm::vec2(AABB.z - AABB.x, AABB.w - AABB.y);
	m_sizeBuffer = m_skin->GetSize();
}


void NoAnimationBodyEntity::ApplyAABB(const glm::vec4 & AABB)
{
	HavingBodyEntity::ApplyAABB(AABB);
	//since the draw function using the pivot at center
	m_skin->SetPos(glm::vec2((AABB.x + AABB.z) / 2.0f, (AABB.y + AABB.w) / 2.0f));
	glm::vec2 scale = glm::vec2(AABB.z - AABB.x, AABB.w - AABB.y) / m_AABBDimensionBuffer;
	m_skin->SetSize(m_sizeBuffer*scale);
}

