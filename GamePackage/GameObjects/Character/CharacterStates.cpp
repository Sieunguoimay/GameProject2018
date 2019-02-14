#include "CharacterStates.h"
#include"CharacterBrain.h"
#include"TerrestrialBody.h"
#include"../../misc/Locator.h"


void BaseCharacterState::Enter(CharacterBrain * entity)
{
	entity->GetSkin()->SetAnimation(m_animationIndex);
}





void _StandingState::Enter(CharacterBrain * entity)
{
	SDL_Log("Standing\n");
	BaseCharacterState::Enter(entity);
}



void _JumpingState::Enter(CharacterBrain * entity)
{
	SDL_Log("Jumping\n");
	BaseCharacterState::Enter(entity);
}

void _JumpingState::Execute(CharacterBrain * entity, float deltaTime)
{
	if (entity->GetBody()->GetBody()->GetLinearVelocity().y < 0)
		entity->ChangeState(entity->GetFallingState());
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

_RunningState::_RunningState(Timeline * pPelvisBone)
	:m_pPelvisBone(pPelvisBone)
{
	m_pelvisAngle = m_pPelvisBone->angleBuffer;
}

void _RunningState::Enter(CharacterBrain * entity)
{

	TerrestrialBody*body = ((TerrestrialBody*)(entity->GetBody()->GetBody()->GetUserData()));

	entity->GetSkin()->GetSpriterEntity()->ControlAngle(m_pPelvisBone,
		calculatePelvisAngle(
			body->GetLeg().GetGroundNormalAngle(),
			entity->GetSkin()->GetFlip() == FlipType::HORIZONTAL_FLIP));

	BaseCharacterState::Enter(entity);

}

void _RunningState::Execute(CharacterBrain * entity, float deltaTime)
{


	TerrestrialBody*body = ((TerrestrialBody*)(entity->GetBody()->GetBody()->GetUserData()));
	if (!body->IsOnGround())return;

	entity->GetSkin()->GetSpriterEntity()->ControlAngle(m_pPelvisBone,
		calculatePelvisAngle(
			body->GetLeg().GetGroundNormalAngle(),
			entity->GetSkin()->GetFlip() == FlipType::HORIZONTAL_FLIP));
}


void _StoppingState::Enter(CharacterBrain * entity)
{
	SDL_Log("Stopping From Running to Standing");
	BaseCharacterState::Enter(entity);
}

void _StoppingState::Execute(CharacterBrain * entity, float deltaTime)
{
	if (entity->GetSkin()->GetSpriterEntity()->HasDone()) {
		entity->ChangeState(entity->m_standing);
	}
}




void _JumpingBufferState::Enter(CharacterBrain * entity)
{
	SDL_Log("jumping buffer");
	BaseCharacterState::Enter(entity);
}

void _JumpingBufferState::Execute(CharacterBrain * entity, float deltaTime)
{
	if (entity->GetSkin()->GetSpriterEntity()->HasDone()) {
		entity->ChangeState(entity->m_jumping);
	}
}

void _FallingState::Enter(CharacterBrain * entity)
{
	SDL_Log("Falling");
	BaseCharacterState::Enter(entity);
}





_GlobalCharacterState::_GlobalCharacterState(std::vector<AnimationBase*>*animations, int size,int timelineIndex)
{
	for (int i = 0; i < size; i++) {
		m_leftHandBone.push_back(((Animation*)animations->at(i))->timelines[timelineIndex]);
	}
}


void _GlobalCharacterState::Enter(CharacterBrain * entity)
{
}

void _GlobalCharacterState::Execute(CharacterBrain * entity, float deltaTime)
{
	SpriterEntity*spriteEntity = entity->GetSkin()->GetSpriterEntity();
		if (Locator::GetInput()->IsKeyPressed(Key::KEY_SPACE)) {
			for (auto&bone : m_leftHandBone) {
				spriteEntity->TakeControl(bone);
				spriteEntity->ControlAngle(bone, 270.0f);
			}
			m_flag = true;
		}
		else {
			if (m_flag) {
				for (auto&bone : m_leftHandBone) {
					spriteEntity->ReleaseControl(bone);
				}
				m_flag = false;
			}
		}


}
