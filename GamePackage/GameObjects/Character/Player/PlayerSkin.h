#pragma once
#include"../../SpriterEntity.h"
class PlayerSkin
{
	class Player*m_pOwner;

	BoneSegment*m_pPelvisBone;
	BoneSegment*m_pArmBone2;
	BoneSegment*m_pArmBone1;
public:
	PlayerSkin(Player*pOwner);
	~PlayerSkin();
	void Update(float deltaTime);
	
	//getters
	BoneSegment*GetArmBone1() { return m_pArmBone1; }
	BoneSegment*GetArmBone2() { return m_pArmBone2; }
	
	//What can I do?
	void LookAt();
	void RaiseHands(const glm::vec2&target,float deltaTime);
	void BendPelvis();


};

