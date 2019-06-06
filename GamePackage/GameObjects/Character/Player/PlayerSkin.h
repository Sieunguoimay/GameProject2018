#pragma once
#include"../../Skin.h"

class PlayerSkin: public AnimationSkin
{
	BodySegment*m_pPelvisBone;
	BodySegment*m_pArmBone2;
	BodySegment*m_pArmBone1;

	bool m_touchTrigger;
	glm::vec2 m_target;
public:
	PlayerSkin(SpriterEntity*spriterEntity, float scale, glm::vec2 pos = glm::vec2(0.0, 0.0), float angle = 0.0);
	~PlayerSkin();
	void Update(float deltaTime)override;
	//What can I do?
	void LookAt();
	void Touch(const glm::vec2&target);
	void BendPelvis();
};

