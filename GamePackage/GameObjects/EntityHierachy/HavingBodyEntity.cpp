#include "HavingBodyEntity.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"

HavingBodyEntity::HavingBodyEntity(/* ObjectType type*/)
	:/*BodyProtocol(type)
	, */m_bodyDragAndDrop(Locator::GetPhysicsFactory()->GetB2World())
{
}

HavingBodyEntity::~HavingBodyEntity()
{
}

void HavingBodyEntity::OnSelect(const glm::vec4 & AABB)
{
	if (m_body == NULL)return;
	if(m_body->GetType()!=b2_staticBody)
		m_bodyDragAndDrop.DragStart(m_body, b2Vec2((AABB.x + AABB.z) / (2.0f*M2P), (AABB.y + AABB.w) / (2.0f*M2P)));
}

void HavingBodyEntity::OnRelease(const glm::vec4 & AABB)
{
	if (m_body == NULL)return;
	if (m_body->GetType() != b2_staticBody) 
		m_bodyDragAndDrop.Drop();

	else{//in case of static body
		Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
		SetupBody(true);
	}
}

void HavingBodyEntity::ApplyAABB(const glm::vec4 & AABB)
{
	if (m_body == NULL)return;
	if (m_body->GetType() != b2_staticBody)
		m_bodyDragAndDrop.SetTarget(b2Vec2((AABB.x + AABB.z) / (2.0f*M2P), (AABB.y + AABB.w) / (2.0f*M2P)));
}



AnimationBodyEntity::AnimationBodyEntity(AnimationSkin * skin, /*ObjectType type,*/ glm::vec4 AABB)
	:/*HavingBodyEntity(type),*/m_skin(skin)
{
	if (m_skin== NULL) return;

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
	if(m_skin) delete m_skin;
}

void AnimationBodyEntity::Update(float deltaTime)
{
	//super
	AABBEntity::Update(deltaTime);
	if (AABBEntity::IsSelected())return;

	b2Vec2 pos = M2P*m_body->GetPosition();
	m_skin->SetPos(glm::vec2(pos.x, pos.y));
	m_skin->SetAngle(m_body->GetAngle());

	m_skin->Update(deltaTime);
}

void AnimationBodyEntity::Draw()
{
	AABBEntity::Draw();
	m_skin->Draw();
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




NoAnimationBodyEntity::NoAnimationBodyEntity(NoAnimationSkin * skin, /*ObjectType type, */glm::vec4 AABB)
	:/*HavingBodyEntity(type),*/m_skin(skin)
{
	if (m_skin == NULL) return;

	//m_skin = (NoAnimationSkin*)m_skinBase;

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
	if (m_skin) delete m_skin;
}

void NoAnimationBodyEntity::Update(float deltaTime)
{
	AABBEntity::Update(deltaTime);
	if (AABBEntity::IsSelected())return;
	b2Vec2 pos = M2P*m_body->GetPosition();
	m_skin->SetPos(glm::vec2(pos.x, pos.y));
	m_skin->SetAngle(m_body->GetAngle());
}

void NoAnimationBodyEntity::Draw()
{
	AABBEntity::Draw();
	m_skin->Draw();
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

