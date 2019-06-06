#include "PlayerSkin.h"


PlayerSkin::PlayerSkin(SpriterEntity * spriterEntity, float scale, glm::vec2 pos, float angle)
	:AnimationSkin(spriterEntity,scale,pos,angle)
{
	m_spriterEntity->SetAnimationSwitchingTime("stand", "run", 200);
	m_spriterEntity->SetAnimationSwitchingTime("stand", "jump", 180);
	m_spriterEntity->SetAnimationSwitchingTime("run", "stand", 200);
	m_spriterEntity->SetAnimationSwitchingTime("run", "jump", 180);
	m_spriterEntity->SetAnimationSwitchingTime("run", "stop", 200);
	m_spriterEntity->SetAnimationSwitchingTime("jump", "run", 300);
	m_spriterEntity->SetAnimationSwitchingTime("jump", "jump2", 20);
	m_spriterEntity->SetAnimationSwitchingTime("jump2", "jump", 20);
	m_spriterEntity->SetAnimationSwitchingTime("stop", "stand", 300);

	m_pArmBone1 = spriterEntity->GetBodySegment(spriterEntity->GetSegmentIndex("bone_012"));
	m_pArmBone2 = spriterEntity->GetBodySegment(spriterEntity->GetSegmentIndex("bone_015"));
	m_pPelvisBone=spriterEntity->GetBodySegment(spriterEntity->GetSegmentIndex("bone_005"));
}

PlayerSkin::~PlayerSkin()
{
}

void PlayerSkin::Update(float deltaTime)
{
	AnimationSkin::Update(deltaTime);
	//touch
	if (m_touchTrigger) {

		if(!m_pArmBone1->IsControlled())
			m_pArmBone1->TakeControl(m_spriterEntity->GetAngleOfBone(m_pArmBone1->GetId()),170,Rotator::CurveType::QUADRATIC);

		if (!m_pArmBone2->IsControlled())
			m_pArmBone2->TakeControl(m_spriterEntity->GetAngleOfBone(m_pArmBone2->GetId()), 170, Rotator::CurveType::QUADRATIC);

		m_target = m_target - m_pos;
		float angle = atan2f(m_target.y, m_target.x)*180.0f/3.1492654f;

		if (GetFlip() == FlipType::HORIZONTAL_FLIP) {
			m_pArmBone1->SetAngle(90.0f - angle, deltaTime);
			m_pArmBone2->SetAngle(90.0f - angle,deltaTime);
		}
		else {
			m_pArmBone1->SetAngle(angle - 90.0f, deltaTime);
			m_pArmBone2->SetAngle(angle - 90.0f, deltaTime);
		}

	}
	else {
		if(m_pArmBone1->IsControlled())
			m_pArmBone1->Release();
		if (m_pArmBone2->IsControlled())
			m_pArmBone2->Release();
	}
	m_touchTrigger = false;
}

void PlayerSkin::LookAt()
{
}

void PlayerSkin::Touch(const glm::vec2&target)
{
	m_touchTrigger = true;
	m_target = target;
}

void PlayerSkin::BendPelvis()
{
}

