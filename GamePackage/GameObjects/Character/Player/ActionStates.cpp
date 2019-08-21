#include "ActionStates.h"
#include"PlayerBody.h"
#include"PlayerSkin.h"
#include"../../../misc/Locator.h"


void BaseActionState::Enter(PlayerBody * entity)
{
	SDL_Log("%s", m_name.c_str());
	entity->m_pSkin->SetAnimation(m_animationIndex);
	m_pPlayerBody = entity;
	m_pPlayerSkin = entity->m_pSkin;
	Enter();
}


void _JumpingState::Execute()
{
	if (m_pPlayerBody->GetBody()->GetLinearVelocity().y < 0)
		m_pPlayerBody->ChangeStateByIndex(AS_FALL);
}

void _StoppingState::Execute()
{
	if (m_pPlayerSkin->GetSpriterEntity()->HasDone()) {
		m_pPlayerBody->ChangeStateByIndex(AS_STAND);
	}
}


void _JumpingBufferState::Execute()
{
	if (m_pPlayerSkin->GetSpriterEntity()->HasDone()) {
		m_pPlayerBody->ChangeStateByIndex(AS_JUMP);
	}
}



void _GlobalCharacterState::Execute()
{
	TouchPoint*touchPoint = m_pPlayerBody->GetNearestTouchPoint();
	if (touchPoint) {
		m_pPlayerSkin->RaiseHands(Utils::b2ToGlm(M2P*touchPoint->GetPos()));
		touchPoint->Draw();
	}
}

//
//float _RunningState::calculatePelvisAngle(float normalAngle, bool flip)
//{
//	float target_angle = 180 + normalAngle;
//	m_pelvisAngle += (target_angle - m_pelvisAngle)*0.05f;
//
//	float pelvisAngle = m_pelvisAngle;
//	if (flip)
//		pelvisAngle = 180.0f - pelvisAngle;
//
//	if (pelvisAngle< 0)pelvisAngle += 360.0f;
//	else if (pelvisAngle >= 360.0f)pelvisAngle -= 360.0f;
//
//	return pelvisAngle;
//}

