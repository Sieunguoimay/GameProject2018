#include "NoBodyEntity.h"



//everything would be based on this first AABB including Body creating
AnimationNoBodyEntity::AnimationNoBodyEntity(SpriterEntity* skin, glm::vec4 AABB)
	:m_spriterEntity(skin)
{
	if (m_spriterEntity == NULL)return;
	//if given AABB has no dimesion spefified, the we copy the dimension of original AABB of SKin;
	if (AABB.z <= AABB.x) {
		const glm::vec4&aabb = m_spriterEntity->GetAABB();
		AABB.z = (aabb.z - aabb.x) + AABB.x;
		AABB.w = (aabb.w - aabb.y) + AABB.y;
	}
	m_spriterEntity->SetAABB(AABB);
}

AnimationNoBodyEntity::~AnimationNoBodyEntity()
{
	if(m_spriterEntity!=NULL)
		delete m_spriterEntity;
}

void AnimationNoBodyEntity::Update(float deltaTime)
{
	//super
	AABBEntity::Update(deltaTime);
	if (AABBEntity::IsSelected())return;

	m_spriterEntity->Update(deltaTime);
}

void AnimationNoBodyEntity::Draw()
{
	//super
	AABBEntity::Draw();
	m_spriterEntity->Draw();

}

void AnimationNoBodyEntity::OnSelect(const glm::vec4 & AABB)
{
	m_animationIndexBuffer = m_spriterEntity->GetCurrentAnimationIndex();
	m_spriterEntity->SetAnimationStrictly(0);
}

void AnimationNoBodyEntity::OnRelease(const glm::vec4 & AABB)
{
	m_spriterEntity->SetAnimation(m_animationIndexBuffer);
}

void AnimationNoBodyEntity::ApplyAABB(const glm::vec4 & AABB)
{
	m_spriterEntity->SetAABB(AABB);
}




NoAnimationNoBodyEntity::NoAnimationNoBodyEntity(NoAnimationSkin * skin, glm::vec2 pos, glm::vec2 size)
	:m_skin(skin)
{
	m_skin->SetPos(pos);
	m_skin->SetSize(size);
	m_skin->SetAngle(3.1415 / 4.0);

}

NoAnimationNoBodyEntity::~NoAnimationNoBodyEntity()
{
	delete m_skin;
}

void NoAnimationNoBodyEntity::Draw()
{
	//super
	AABBEntity::Draw();
	m_skin->Draw();
}

void NoAnimationNoBodyEntity::OnSelect(const glm::vec4 & AABB)
{
	m_AABBDimensionBuffer = glm::vec2(AABB.z - AABB.x, AABB.w - AABB.y);
	m_sizeBuffer = m_skin->GetSize();
}

void NoAnimationNoBodyEntity::OnRelease(const glm::vec4 & AABB)
{
}

inline void NoAnimationNoBodyEntity::ApplyAABB(const glm::vec4 & AABB)
{
	//since the draw function using the pivot at center
	m_skin->SetPos(glm::vec2((AABB.x+AABB.z)/2.0f,(AABB.y+AABB.w)/2.0f));
	glm::vec2 scale = glm::vec2(AABB.z - AABB.x, AABB.w - AABB.y) / m_AABBDimensionBuffer;
	m_skin->SetSize(m_sizeBuffer*scale);
}
