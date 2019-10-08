#include "BrainStates.h"
#include"Player.h"

void BrainStateOnTheFloor::Enter()
{
	SDL_Log("On the floor");
}

void BrainStateOnTheFloor::Execute(float deltaTime)
{
	if (d->m_control.m_jump) {
		if (d->m_control.m_left) d->m_actuator.JumpLeft();
		else if (d->m_control.m_right) d->m_actuator.JumpRight();
		else d->m_actuator.Jump();
	}
	else if (!d->m_actuator.CheckInStates(AS_JUMP,AS_JUMP_LEFT,AS_JUMP_RIGHT,-1)) {
		if (d->m_control.m_left) d->m_actuator.RunLeft();
		if (d->m_control.m_right) d->m_actuator.RunRight();
	}

	if (d->m_actuator.CheckInState(AS_RUN_LEFT) ||
		d->m_actuator.CheckInState(AS_RUN_RIGHT))
		if (!d->m_control.m_left && !d->m_control.m_right) 
			d->m_actuator.Stop();

	if (d->m_actuator.CheckInState(AS_FALL))
		d->m_actuator.Stand();


	//change the state here according to the perception
	auto&percepts = d->m_memory.GetPercepts();
	for (auto& p : percepts) {
		if (p.m_type == PPT_CLOSEST_ROCK) {
			if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_NEXT_TO_ROCK_RHS) {
				d->m_decisionMaker.ChangeBrainState(BS_NEXT_TO_ROCK_RHS);
				return;
			}else if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_NEXT_TO_ROCK_LHS) {
				d->m_decisionMaker.ChangeBrainState(BS_NEXT_TO_ROCK_LHS);
				return;
			}
			else if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_ON_ROCK_TOP_LEFT) {
				d->m_decisionMaker.ChangeBrainState(BS_ON_ROCK_TOP_LEFT);
				return;
			}
			else if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_ON_ROCK_TOP_RIGHT) {
				d->m_decisionMaker.ChangeBrainState(BS_ON_ROCK_TOP_RIGHT);
				return;
			}
		}
	}
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
	d->m_actuator.RaiseHands(d->m_memory.GetPercept(PPT_CLOSEST_TOUCH_POINT),deltaTime);
	
	auto closestRockPercept = d->m_memory.GetPercept(PPT_CLOSEST_ROCK);

	if (d->m_control.m_jump) {
		if (d->m_control.m_right) d->m_actuator.JumpRight();
		else d->m_actuator.ClimbOnRock(closestRockPercept);
	}
	else if (!d->m_actuator.CheckInState(AS_JUMP)) {
		if (d->m_control.m_left) {
			if (closestRockPercept != NULL) {
				if (closestRockPercept->m_sensingMechanism == SM_TOUCHING)
					d->m_actuator.PushLeft();
				else
					d->m_actuator.RunLeft();
			}
			return;
		}else if (d->m_control.m_right) {
			if(d->m_control.m_holdTouchPointNow)
				d->m_actuator.PullRight(d->m_memory.GetPercept(PPT_CLOSEST_TOUCH_POINT));
			else
				d->m_actuator.RunRight();
		}
	}

	if (d->m_actuator.CheckInStates (
		AS_RUN_LEFT,AS_RUN_RIGHT, AS_PULL_LEFT, AS_PULL_RIGHT, 
		AS_PUSH_LEFT, AS_PUSH_RIGHT,-1))
		if (!d->m_control.m_left && !d->m_control.m_right) d->m_actuator.Stop();

	if (d->m_actuator.CheckInState(AS_FALL))
		d->m_actuator.Stand();


	auto&percepts = d->m_memory.GetPercepts();
	for (auto& p : percepts) {
		if (p.m_type == PPT_CLOSEST_ROCK) {
			if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_NEXT_TO_ROCK_LHS) {
				return;
			}
		}
	}
	d->m_decisionMaker.ChangeBrainState(BS_ON_THE_FLOOR);
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
	d->m_actuator.RaiseHands(d->m_memory.GetPercept(PPT_CLOSEST_TOUCH_POINT), deltaTime);

	auto closestRockPercept = d->m_memory.GetPercept(PPT_CLOSEST_ROCK);

	if (d->m_control.m_jump) {
		if (d->m_control.m_left) d->m_actuator.JumpLeft();
		else d->m_actuator.ClimbOnRock(closestRockPercept);
	}
	else if (!d->m_actuator.CheckInState(AS_JUMP)) {
		if (d->m_control.m_right) {
			if (closestRockPercept != NULL) {
				if(closestRockPercept->m_sensingMechanism == SM_TOUCHING)
					d->m_actuator.PushRight();
				else
					d->m_actuator.RunRight();
			}
			return;
		}
		else if (d->m_control.m_left) {
			if (d->m_control.m_holdTouchPointNow)
				d->m_actuator.PullLeft(d->m_memory.GetPercept(PPT_CLOSEST_TOUCH_POINT));
			else
				d->m_actuator.RunLeft();
		}
	}
	if (d->m_actuator.CheckInStates(
		AS_RUN_LEFT, AS_RUN_RIGHT, AS_PULL_LEFT, AS_PULL_RIGHT,
		AS_PUSH_LEFT, AS_PUSH_RIGHT, -1))
		if (!d->m_control.m_left && !d->m_control.m_right) d->m_actuator.Stop();

	if (d->m_actuator.CheckInState(AS_FALL))
		d->m_actuator.Stand();


	auto&percepts = d->m_memory.GetPercepts();
	for (auto& p : percepts) {
		if (p.m_type == PPT_CLOSEST_ROCK) {
			if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_NEXT_TO_ROCK_RHS) {
				return;
			}
		}
	}
	d->m_decisionMaker.ChangeBrainState(BS_ON_THE_FLOOR);
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
	if (d->m_control.m_jump) {
		if (d->m_control.m_left) d->m_actuator.JumpLeft();
		else if (d->m_control.m_right) d->m_actuator.JumpRight();
		else d->m_actuator.Jump();
	}
	else if (!d->m_actuator.CheckInStates(AS_JUMP, AS_JUMP_LEFT, AS_JUMP_RIGHT, -1)) {
		if (d->m_control.m_left) 
			d->m_actuator.RunLeft();
		if (d->m_control.m_right&&d->m_actuator.CheckInState(AS_STAND)) {
			d->m_actuator.ClimbDownRockRight(d->m_memory.GetPercept(PPT_CLOSEST_ROCK));
		}
	}

	if (d->m_actuator.CheckInState(AS_RUN_LEFT) ||
		d->m_actuator.CheckInState(AS_RUN_RIGHT))
		if (!d->m_control.m_left && !d->m_control.m_right) d->m_actuator.Stop();

	if (d->m_actuator.CheckInState(AS_FALL))
		d->m_actuator.Stand();


	auto&percepts = d->m_memory.GetPercepts();
	for (auto& p : percepts) {
		if (p.m_type == PPT_CLOSEST_ROCK) {
			if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_ON_ROCK_TOP_RIGHT) {
				return;
			}
		}
	}
	d->m_decisionMaker.ChangeBrainState(BS_ON_THE_FLOOR);
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
	if (d->m_control.m_jump) {
		if (d->m_control.m_left) d->m_actuator.JumpLeft();
		else if (d->m_control.m_right) d->m_actuator.JumpRight();
		else d->m_actuator.Jump();
	}
	else if (!d->m_actuator.CheckInStates(AS_JUMP, AS_JUMP_LEFT, AS_JUMP_RIGHT, -1)) {
		if (d->m_control.m_left&&d->m_actuator.CheckInState(AS_STAND)) {
			d->m_actuator.ClimbDownRockLeft(d->m_memory.GetPercept(PPT_CLOSEST_ROCK));
		}
		if (d->m_control.m_right) d->m_actuator.RunRight();
	}

	if (d->m_actuator.CheckInState(AS_RUN_LEFT) ||
		d->m_actuator.CheckInState(AS_RUN_RIGHT))
		if (!d->m_control.m_left && !d->m_control.m_right) d->m_actuator.Stop();

	if (d->m_actuator.CheckInState(AS_FALL))
		d->m_actuator.Stand();


	auto&percepts = d->m_memory.GetPercepts();
	for (auto& p : percepts) {
		if (p.m_type == PPT_CLOSEST_ROCK) {
			if (p.m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_ON_ROCK_TOP_LEFT) {
				return;
			}
		}
	}
	d->m_decisionMaker.ChangeBrainState(BS_ON_THE_FLOOR);
}

void BrainStateOnRockTopLeft::Exit()
{
}



BrainStateLongAction::~BrainStateLongAction()
{
	while (!m_actions.empty()) {
		if(m_actions.front()->IsAutorelease())
			delete m_actions.front();
		m_actions.pop();
	}
}

void BrainStateLongAction::Enter()
{
	m_enterFlag = true;
	SDL_Log("BrainStateLongAction Enters");
}

void BrainStateLongAction::Execute(float deltaTime)
{
	if (!m_actions.empty()) {
		if (m_enterFlag) {
			m_actions.front()->Enter();
			m_enterFlag = false;
		}
		m_actions.front()->Run(deltaTime,d->m_memory.GetPercept(PPT_CLOSEST_ROCK));
		if (m_actions.front()->Timeout()) {
			m_actions.front()->Exit();
			if (m_actions.front()->IsAutorelease()) {
				delete m_actions.front();
			}
			m_actions.pop();
			m_enterFlag = true;
		}
	}
	if(m_actions.empty()) {
		if (m_followState != NULL)
			d->m_decisionMaker.ChangeState(m_followState);
	}
}

void BrainStateLongAction::Exit()
{
}
