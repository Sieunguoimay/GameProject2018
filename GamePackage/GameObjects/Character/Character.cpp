#include "Character.h"
#include"../../misc/Assistances.h"





Skin::Skin(SpriterEntity * spriterEntity, float scale, glm::vec2 pos /*= glm::vec2(0.0, 0.0)*/, float angle /*= 0.0*/)
	:m_spriterEntity(spriterEntity),m_scale(scale),m_pos(pos),m_angle(angle)
	, m_flip(FLIP_NONE)
{
}

Skin::~Skin()
{
	delete m_spriterEntity;//clue of the single-copy property.
}

void Skin::SetAnimation(std::string name)
{
	m_spriterEntity->SetAnimation(name);
}

void Skin::SetPos(glm::vec2 pos)
{
	m_pos = pos;
}

void Skin::SetAngle(float angle)
{
	m_angle = angle;
}

void Skin::SetScale(float scale)
{
	m_scale = scale;
}

void Skin::SetFlip(FlipType flip)
{
	m_flip = flip;
}

void Skin::Update(float deltaTime)
{
	m_spriterEntity->Update(deltaTime);
}

void Skin::Draw()
{
	m_spriterEntity->Draw(m_pos, m_angle, m_scale, m_scale,m_flip);
}

SpriterEntity * Skin::GetSpriterEntity()
{
	return m_spriterEntity;
}

void Skin::SetAnimationSwitchingTime(std::string animationA, std::string animationB, int time)
{
	m_spriterEntity->GetSclmEntity()->animationSwitchingTime
		.at(m_spriterEntity->GetAnimationIndex(animationA))
		.at(m_spriterEntity->GetAnimationIndex(animationB)) = time;

}







Brain::Brain(BodyBase*body, Skin*skin)
	:m_pBody(body)
	,m_pSkin(skin)
{
}

void Brain::Update(float deltaTime)
{
	//body.SetLinearVelocity(10, 0);
}












Character::Character(Brain * brain, BodyBase* body, Skin * skin)
	:m_brain(brain),m_body(body),m_skin(skin)
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

	m_skin->SetPos(m_body->GetBodyKit().GetPosition());
	m_skin->SetAngle(glm::degrees(m_body->GetBodyKit().GetAngle()));
}

void Character::Draw()
{
	m_skin->Draw();
}
