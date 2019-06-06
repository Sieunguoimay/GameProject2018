#pragma once
#include"../EntityHierachy/HavingBodyEntity.h"

class Animal :public AnimationBodyEntity{
public:
	Animal(const glm::vec2& pos = glm::vec2(0, 0));
	Animal(const glm::vec4&aabb);
	~Animal();
	void SetupBody(bool atRunTime = false)override;
	//void OnBeginContact(b2Contact*contact, b2Fixture*fixture)override;
	void Update(float deltaTime)override;

	Spawner*Spawn(InfoPacket*info)override;
};

class Rock :public NoAnimationBodyEntity {
public:
	Rock();
	void SetupBody(bool atRunTime = false)override;

};

#include"../EntityHierachy/NoBodyEntity.h"
class Grass : public AnimationNoBodyEntity {
public:
	Grass(const glm::vec2&pos=glm::vec2(0,0));
	Spawner*Spawn(InfoPacket*info)override;
};

class Leaf :public NoAnimationNoBodyEntity {
public:
	Leaf(const glm::vec2&pos = glm::vec2(0, 0));
	Spawner*Spawn(InfoPacket*info)override;
};

