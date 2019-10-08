#pragma once
#include<vector>
#include"Percept.h"

//what player can perceive
enum PlayerPerceptType {
	PPT_CLOSEST_ROCK,
	PPT_CLOSEST_TOUCH_POINT,
	PPT_TOTAL_NEAREST_PERCEPT_NUM
};
//maximum number of percept sabout surrounding objects that PlayerMemory can store
#define MEMORY_CAPACITY 10

class PlayerMemory {
public:
	enum ClosestRockFlag {
		BODY_STATE_NEXT_TO_ROCK_LHS,
		BODY_STATE_NEXT_TO_ROCK_RHS,
		BODY_STATE_ON_ROCK_TOP_LEFT,
		BODY_STATE_ON_ROCK_TOP_RIGHT
	};

private:
	int m_indices[PPT_TOTAL_NEAREST_PERCEPT_NUM];

	class Player*m_pOwner;
	std::vector<Percept>m_percepts;	
public:
	PlayerMemory(Player*pOwner);
	~PlayerMemory();


	//Sensor can modify memory
	void AddPercept(Percept percept);
	void AddPercept(const Percept& percept,int which);
	void ClearAllPercepts();

	//Brain (DecisionMaker) can read memory
	std::vector<Percept>&GetPercepts();
	Percept*GetPercept(int which);
};









inline void PlayerMemory::ClearAllPercepts(){
	m_percepts.clear();
	for(auto&i:m_indices)i = -1;
}
inline std::vector<Percept>&PlayerMemory::GetPercepts() {
	return m_percepts;
}
inline Percept*PlayerMemory::GetPercept(int which) {
	return (m_indices[which]==-1? NULL : &m_percepts[m_indices[which]]);
}

