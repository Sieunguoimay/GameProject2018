#include "NoBodyEntity.h"

AnimationNoBodyEntity::AnimationNoBodyEntity(AnimationSkin * skin, glm::vec2 m_pos)
	:m_skin(skin)
{
	m_skin->SetPos(m_pos);
}

AnimationNoBodyEntity::~AnimationNoBodyEntity()
{
	delete m_skin;
}

void AnimationNoBodyEntity::Update(float deltaTime)
{
	//super
	AABBEntity::Update(deltaTime);
	m_skin->Update(deltaTime);

	//calculate AABB
	SetAABB(
		m_skin->GetPosition() - 0.5f*m_skin->GetSize(),
		m_skin->GetPosition() + 0.5f*m_skin->GetSize());

}

void AnimationNoBodyEntity::Draw()
{
	//super
	AABBEntity::Draw();
	m_skin->Draw();
}

void AnimationNoBodyEntity::OnSelect(const glm::vec2 & AA, const glm::vec2 & BB)
{
}

void AnimationNoBodyEntity::OnRelease(const glm::vec2 & AA, const glm::vec2 & BB)
{
}





NoAnimationNoBodyEntity::NoAnimationNoBodyEntity(NoAnimationSkin * skin, glm::vec2 pos, glm::vec2 size)
	:m_skin(skin)
{
	m_skin->SetPos(pos);
	m_skin->SetSize(size);
}

NoAnimationNoBodyEntity::~NoAnimationNoBodyEntity()
{
	delete m_skin;
}

void NoAnimationNoBodyEntity::Draw()
{
	//calculate AABB
	SetAABB(
		m_skin->GetPosition() - 0.5f*m_skin->GetSize(),
		m_skin->GetPosition() + 0.5f*m_skin->GetSize());

	//super
	AABBEntity::Draw();
	m_skin->Draw();
}

void NoAnimationNoBodyEntity::OnSelect(const glm::vec2 & AA, const glm::vec2 & BB)
{
}

void NoAnimationNoBodyEntity::OnRelease(const glm::vec2 & AA, const glm::vec2 & BB)
{
}
