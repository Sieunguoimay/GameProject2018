#include "Character.h"
#include"../../misc/Assistances.h"


Brain::Brain(BodyBase*body, AnimationSkin*AnimationSkin)
	:m_pBody(body)
	,m_pSkin(AnimationSkin)
{
}

void Brain::Update(float deltaTime)
{
	//body.SetLinearVelocity(10, 0);
}












Character::Character(Brain * brain, BodyBase* body, AnimationSkin * AnimationSkin)
	:m_brain(brain),m_body(body),m_skin(AnimationSkin)
{
}

Character::~Character()
{
	if (m_brain) delete m_brain;
	if (m_body) delete m_body;
	if (m_skin) delete m_skin;
}

void Character::Update(float deltaTime)
{
	m_brain->Update(deltaTime);
	m_body->Update(deltaTime);
	m_skin->Update(deltaTime);

	m_skin->SetPos(*m_body->GetPosPointer());
	m_skin->SetAngle(glm::degrees(m_body->GetBody()->GetAngle()));
}

void Character::Draw()
{
	m_skin->Draw();
}
