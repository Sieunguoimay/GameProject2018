#pragma once
#include"../EntityHierachy/HavingBodyEntity.h"
class Animal :public AnimationBodyEntity{
	glm::vec2 m_pos;
public:
	Animal(glm::vec2 pos = glm::vec2(0,0));
	~Animal();
	void SetupBody()override;
	void OnBeginContact(b2Contact*contact, b2Fixture*fixture)override;
	void Update(float deltaTime)override;
};

class Rock :public NoAnimationBodyEntity {
public:
	Rock();
	void SetupBody()override;
};

#include"../EntityHierachy/NoBodyEntity.h"
class Grass : public AnimationNoBodyEntity {
public:
	Grass();
};

class Leaf :public NoAnimationNoBodyEntity {
public:
	Leaf();
};

#include"../EntityHierachy/MultiEntity.h"
class FlockOfAnimal :public MultiEntity<AnimationBodyEntity>{
public:
	FlockOfAnimal();
};
