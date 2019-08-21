#include "BrainStates.h"
#include"Player.h"

void BrainStateOnTheFloor::Enter()
{
	SDL_Log("On the floor");
}

void BrainStateOnTheFloor::Execute(float deltaTime)
{
	if (d->m_jump) {
		if (d->m_left) d->m_playerBody->JumpLeft();
		else if (d->m_right) d->m_playerBody->JumpRight();
		else d->m_playerBody->Jump();
	}
	else if (!d->m_playerBody->CheckInState(AS_JUMP)) {
		if (d->m_left) d->m_playerBody->RunLeft();
		if (d->m_right) d->m_playerBody->RunRight();
	}

	if (d->m_playerBody->CheckInState(AS_RUN_LEFT) ||
		d->m_playerBody->CheckInState(AS_RUN_RIGHT))
		if (!d->m_left && !d->m_right) d->m_playerBody->Stop();

	if (d->m_playerBody->CheckInState(AS_FALL))
		d->m_playerBody->Stand();
}

void BrainStateOnTheFloor::Exit()
{
}



void BrainStateNextToRockLHS::Enter()
{
	SDL_Log("Next to rock LHS");
}

void BrainStateNextToRockLHS::Execute(float deltaTime)
{
	if (d->m_jump) {
		if (d->m_left) d->m_playerBody->ClimbOnLeft();
		else if (d->m_right) d->m_playerBody->JumpRight();
		else d->m_playerBody->ClimbOnLeft();
	}
	else if (!d->m_playerBody->CheckInState(AS_JUMP)) {
		if (d->m_left) {
			d->m_playerBody->PushLeft();
			return;
		}else if (d->m_right) {
			if(d->m_holdTouchPointNow)
				d->m_playerBody->PullRight();
			else
				d->m_playerBody->RunRight();
			return;
		}
	}

	if (d->m_playerBody->CheckInStates (
		AS_RUN_LEFT,AS_RUN_RIGHT, AS_PULL_LEFT, AS_PULL_RIGHT, 
		AS_PUSH_LEFT, AS_PUSH_RIGHT,-1))
		if (!d->m_left && !d->m_right) d->m_playerBody->Stop();

	if (d->m_playerBody->CheckInState(AS_FALL))
		d->m_playerBody->Stand();

}

void BrainStateNextToRockLHS::Exit()
{
}



void BrainStateNextToRockRHS::Enter()
{
	SDL_Log("Next to rock RHS");
}

void BrainStateNextToRockRHS::Execute(float deltaTime)
{
}

void BrainStateNextToRockRHS::Exit()
{
}



void BrainStateOnRockTopRight::Enter()
{
	SDL_Log("On rock top right");
}

void BrainStateOnRockTopRight::Execute(float deltaTime)
{
}

void BrainStateOnRockTopRight::Exit()
{
}



void BrainStateOnRockTopLeft::Enter()
{
	SDL_Log("On rock top left");
}

void BrainStateOnRockTopLeft::Execute(float deltaTime)
{
}

void BrainStateOnRockTopLeft::Exit()
{
}




void BrainStateTest::Enter()
{
	SDL_Log("Test state");
}

void BrainStateTest::Execute(float deltaTime)
{
	if (d->m_jump) {
		d->m_playerBody->ClimbOnLeft();
	}
}

void BrainStateTest::Exit()
{
}
