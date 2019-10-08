#pragma once
#include"BrainStates.h"
#include"PlayerMemory.h"

class PlayerDecisionMaker:public StateMachine<Player> {
	class Player*m_pOwner;
	BrainState* m_brainStates[BrainStateEnum::BS_TOTAL_NUM];

public:
	PlayerDecisionMaker(Player*pOwner);
	~PlayerDecisionMaker();
	void Update(float deltaTime);

	//callback implementation
	void ChangeBrainState(int state) { ChangeState(m_brainStates[state]); }
	BrainState*GetState(int state) { return m_brainStates[state]; }
};
