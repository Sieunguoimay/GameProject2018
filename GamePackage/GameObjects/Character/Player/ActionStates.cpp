#include "ActionStates.h"
#include"Player.h"
#include"../../../misc/Locator.h"


void BaseActionState::Enter(PlayerActuator * entity)
{
	SDL_Log("%s %d", m_name.c_str(),m_flipFlag);

	if (m_setAnimationOnEnterFlag)
		entity->m_pOwner->GetSpriterEntity()->SetAnimation(m_animationIndex);

	if (m_flipFlag != -1) 
		entity->m_pOwner->GetSpriterEntity()->SetFlip(m_flipFlag ? HORIZONTAL_FLIP : FLIP_NONE);

	m_pPlayerActuator = entity;
	Enter();
}



void _JumpingState::Execute()
{
	if (m_pPlayerActuator->m_pOwner->GetBody()->GetLinearVelocity().y < 0)
		m_pPlayerActuator->ChangeStateByIndex(AS_FALL);
}


void _StoppingState::Execute()
{
	if (m_pPlayerActuator->m_pOwner->GetSpriterEntity()->HasDone()) {
		m_pPlayerActuator->ChangeStateByIndex(AS_STAND);
	}
}


void _JumpingBufferState::Execute()
{
	if (m_pPlayerActuator->m_pOwner->GetSpriterEntity()->HasDone()) {
		m_pPlayerActuator->ChangeStateByIndex(AS_JUMP);
	}
}




_ClimbingState::_ClimbingState(int preIndex, int index, int flip)
	:BaseActionState(preIndex, flip)
{
	(m_nextState = new BaseActionState(index,flip))->SetName("Climbing");
}

_ClimbingState::~_ClimbingState()
{
	delete m_nextState;
}


void _ClimbingState::Execute()
{
	if (m_pPlayerActuator->m_pOwner->GetSpriterEntity()->HasDone()) {
		m_pPlayerActuator->ChangeState(m_nextState);
	}
}

