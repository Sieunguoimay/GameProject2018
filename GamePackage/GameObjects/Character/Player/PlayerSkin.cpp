#include "PlayerSkin.h"
#include"Player.h"

PlayerSkin::PlayerSkin(Player*pOwner)
	:m_pOwner(pOwner)
{
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("stand", "run", 200);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("stand", "jump", 180);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("run", "stand", 200);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("run", "jump", 180);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("run", "stop", 200);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("jump", "run", 300);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("jump", "jump2", 20);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("jump2", "jump", 20);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("stop", "stand", 300);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("stand", "pre_climb", 300);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("climb", "stand", 200);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("pre_climb", "climb", 300);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("climb_down", "stand", 300);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("push", "stand", 500);
	m_pOwner->m_spriterEntity->SetAnimationSwitchingTime("stand", "push", 500);

	m_pArmBone1		= m_pOwner->m_spriterEntity->GetBodySegment(m_pOwner->m_spriterEntity->GetSegmentIndex("bone_012"));
	m_pArmBone2		= m_pOwner->m_spriterEntity->GetBodySegment(m_pOwner->m_spriterEntity->GetSegmentIndex("bone_015"));
	m_pPelvisBone	= m_pOwner->m_spriterEntity->GetBodySegment(m_pOwner->m_spriterEntity->GetSegmentIndex("bone_005"));
}

PlayerSkin::~PlayerSkin()
{
}

void PlayerSkin::Update(float deltaTime)
{
}

void PlayerSkin::LookAt()
{
}

void PlayerSkin::RaiseHands(const glm::vec2&target, float deltaTime)
{
	glm::vec2 diff = target - 0.5f*(m_pArmBone1->GetPos() + m_pArmBone2->GetPos());
	float angle = atan2f(diff.y, diff.x)*180.0f / 3.1492654f;
	
	m_pArmBone1->SteerAngle(angle, deltaTime, 135, Rotator::CurveType::QUADRATIC);
	m_pArmBone2->SteerAngle(angle, deltaTime, 135, Rotator::CurveType::QUADRATIC);
}

void PlayerSkin::BendPelvis()
{
}

