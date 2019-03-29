#include "Character.h"
Character::Character(AnimationSkin * animationSkin, const glm::vec4 & AABB)
	: AnimationBodyEntity(animationSkin, ObjectType::NULL_TYPE, AABB) {}

Character::~Character()
{
	if (m_brain) delete m_brain;
	if (m_bodyBase) delete m_bodyBase;
}

void Character::Update(float deltaTime)
{
	m_brain->Update(deltaTime);
	AnimationBodyEntity::Update(deltaTime);
}

