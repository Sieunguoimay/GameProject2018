#include "CharacterStates.h"
#include"Player.h"
#include"TerrestrialBody.h"
#include"../../../misc/Locator.h"


void BaseCharacterState::Enter(Player * entity)
{
	entity->GetSkin()->SetAnimation(m_animationIndex);
}



void _StandingState::Enter(Player * entity)
{
	SDL_Log("Standing\n");
	BaseCharacterState::Enter(entity);
}



void _JumpingState::Enter(Player * entity)
{
	SDL_Log("Jumping\n");
	BaseCharacterState::Enter(entity);
}

void _JumpingState::Execute(Player * entity, float deltaTime)
{
	if (entity->GetTerrestrialBody()->GetBody()->GetLinearVelocity().y < 0)
		entity->ChangeState(entity->m_falling);
}



float _RunningState::calculatePelvisAngle(float normalAngle, bool flip)
{

	float target_angle = 180 + normalAngle;
	m_pelvisAngle += (target_angle - m_pelvisAngle)*0.05f;

	float pelvisAngle = m_pelvisAngle;
	if (flip)
		pelvisAngle = 180.0f - pelvisAngle;

	if (pelvisAngle< 0)pelvisAngle += 360.0f;
	else if (pelvisAngle >= 360.0f)pelvisAngle -= 360.0f;

	return pelvisAngle;
}

_RunningState::_RunningState(int pPelvisBone, Player * entity)
{
	//m_pPelvisBone = entity->GetSkin()->GetSpriterEntity()->GetControlKey(pPelvisBone);
	//m_pelvisAngle = entity->GetSkin()->GetSpriterEntity()->GetAngleOfBone(pPelvisBone);
}

void _RunningState::Enter(Player * entity)
{
	SDL_Log("Running\n");

	//m_pPelvisBone->TakeControl(calculatePelvisAngle(
	//	entity->GetTerrestrialBody()->GetGroundNormalAngle(),
	//	entity->GetSkin()->GetFlip() == FlipType::HORIZONTAL_FLIP),135,Rotator::CurveType::LINEAR);
/*
	entity->GetSkin()->GetSpriterEntity()->TakeControl(m_pPelvisBone);
	entity->GetSkin()->GetSpriterEntity()->ControlAngle(m_pPelvisBone,
		calculatePelvisAngle(
			body->GetGroundNormalAngle(),
			entity->GetSkin()->GetFlip() == FlipType::HORIZONTAL_FLIP));
*/
	BaseCharacterState::Enter(entity);


}

void _RunningState::Execute(Player * entity, float deltaTime)
{
	TerrestrialBody*body = entity->GetTerrestrialBody();
	if (!body->IsOnGround())return;

	//m_pPelvisBone->SetAngle(calculatePelvisAngle(
	//		body->GetGroundNormalAngle(),
	//		entity->GetSkin()->GetFlip() == FlipType::HORIZONTAL_FLIP),deltaTime);
}

void _RunningState::Exit(Player * entity)
{
	//m_pPelvisBone->Release();
}


void _StoppingState::Enter(Player * entity)
{
	SDL_Log("Stopping From Running to Standing");
	BaseCharacterState::Enter(entity);
}

void _StoppingState::Execute(Player * entity, float deltaTime)
{
	if (entity->GetSkin()->GetSpriterEntity()->HasDone()) {
		entity->ChangeState(entity->m_standing);
	}
}




void _JumpingBufferState::Enter(Player * entity)
{
	SDL_Log("jumping buffer");
	BaseCharacterState::Enter(entity);
}

void _JumpingBufferState::Execute(Player * entity, float deltaTime)
{
	if (entity->GetSkin()->GetSpriterEntity()->HasDone()) {
		entity->ChangeState(entity->m_jumping);
	}
}

void _FallingState::Enter(Player * entity)
{
	SDL_Log("Falling");
	BaseCharacterState::Enter(entity);
}




_GlobalCharacterState::_GlobalCharacterState(int boneIndex)
	:m_boneIndex(boneIndex)
{
}


void _GlobalCharacterState::Enter(Player * entity)
{
}

void _GlobalCharacterState::Execute(Player * entity, float deltaTime)
{
	PlayerSkin*playerSkin = ((PlayerSkin*)(entity->GetSkin()));
	//SpriterEntity*spriteEntity = entity->GetSkin()->GetSpriterEntity();
	//if (entity->m_armAngle!=-1) {
	//	if (!m_flag)
	//		spriteEntity->TakeControl(m_boneIndex);
	//	

	//	spriteEntity->ControlAngle(m_boneIndex, entity->m_armAngle);

	//	m_flag = true;
	//}
	//else {
	//	if (m_flag) {
	//		spriteEntity->ReleaseControl(m_boneIndex);
	//		m_flag = false;
	//	}
	//}


	TouchPoint*touchPoint = entity->GetTerrestrialBody()->GetTouchPoint();
	if (touchPoint) {
		playerSkin->Touch(b2ToGlm(M2P*touchPoint->GetPos()));
	}
}
