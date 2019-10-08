#include "PlayerDecisionMaker.h"
#include"Player.h"
#include"../../../misc/Math/Geometry.h"
PlayerDecisionMaker::PlayerDecisionMaker(Player*pOwner) :m_pOwner(pOwner) {

	m_brainStates[BS_ON_THE_FLOOR] = new BrainStateOnTheFloor();
	m_brainStates[BS_NEXT_TO_ROCK_LHS] = new BrainStateNextToRockLHS();
	m_brainStates[BS_NEXT_TO_ROCK_RHS] = new BrainStateNextToRockRHS();
	m_brainStates[BS_ON_ROCK_TOP_LEFT] = new BrainStateOnRockTopLeft();
	m_brainStates[BS_ON_ROCK_TOP_RIGHT] = new BrainStateOnRockTopRight();
	m_brainStates[BS_LONG_ACTION] = new BrainStateLongAction();
	StateMachine<Player>::Init(m_pOwner, m_brainStates[BS_ON_THE_FLOOR], NULL, NULL);
}

PlayerDecisionMaker::~PlayerDecisionMaker()
{
	for (int i = 0; i < BS_TOTAL_NUM; i++)
		delete m_brainStates[i];
}

void PlayerDecisionMaker::Update(float deltaTime)
{
	StateMachine<Player>::Update(deltaTime);
}
