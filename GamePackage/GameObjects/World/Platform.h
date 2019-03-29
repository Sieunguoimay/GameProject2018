#pragma once
#include"../EntityHierachy/HavingBodyEntity.h"
class Platform :public NoAnimationBodyEntity, public InfoPacket {
	glm::vec2 m_originalDimension;
public:
	//for the prototype
	Platform();

	Platform(Texture*pTexture,const glm::vec4& AABB);
	~Platform();

	Spawner*Spawn(InfoPacket*info)override;

	void Update(float deltaTime)override;
	void SetupBody(bool atRunTime = false)override;

	//unchanged from the beginning till the end
	std::vector<std::vector<b2Vec2>>m_polygons;
	glm::vec2 m_scaleBuffer;
};
