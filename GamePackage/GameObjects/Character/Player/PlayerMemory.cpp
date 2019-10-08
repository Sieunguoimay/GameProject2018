#include "PlayerMemory.h"
#include"BrainStates.h"
#include"Player.h"
PlayerMemory::PlayerMemory(Player*pOwner)
	: m_pOwner(pOwner)
{
	m_percepts.reserve(MEMORY_CAPACITY);
}

PlayerMemory::~PlayerMemory()
{
}


void PlayerMemory::AddPercept(Percept percept)
{
	if (m_percepts.size() >= MEMORY_CAPACITY)return;
	m_percepts.push_back(percept);
}

void PlayerMemory::AddPercept(const Percept & percept, int which)
{
	if (m_indices[which] == -1) {
		m_indices[which] = m_percepts.size();
		m_percepts.push_back(percept);
	}
	else {
		m_percepts[m_indices[which]] = percept;
	}
}
