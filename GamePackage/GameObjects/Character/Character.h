#pragma once
#include"Box2D\Box2D.h"
#include"../Entity.h"
#include"../SpriterEntity.h"
#include"../PhysicsEngine/BodyBase.h"






//why skin???
//-> because the m_spriterEntity is unque for that specific ScmlEntity
//and it does not keep the information about position or other setup

//for example, when you want to pass the position from body into the spriter
//at update. how can you do that without a memory in between to transfer
//that data to the draw function...

class Skin {
	SpriterEntity*m_spriterEntity;

	float m_scale;
	float m_angle;
	glm::vec2 m_pos;
	FlipType m_flip;
public:
	Skin(SpriterEntity*spriterEntity,float scale, glm::vec2 pos = glm::vec2(0.0,0.0), float angle = 0.0);
	~Skin();
	void SetAnimation(int index);
	void SetPos(glm::vec2 pos);
	void SetAngle(float angle);
	void SetScale(float scale);
	void SetFlip(FlipType flip);
	void Update(float deltaTime);
	void Draw();
	SpriterEntity*GetSpriterEntity();
	void SetAnimationSwitchingTime(const char*animationA, const char*animationB, int time);
	inline glm::vec2*GetPosPointer() { return &m_pos; }
	inline const FlipType& GetFlip()const { return m_flip; }
};

class Brain {

public:
	Brain(BodyBase*body, Skin*skin);
	virtual ~Brain(){}
	virtual void Update(float deltaTime);

	
	
	Skin*GetSkin() { return m_pSkin; }
	BodyBase*GetBody() { return m_pBody; }
protected:	
	BodyBase*m_pBody;
	Skin*m_pSkin;
};




class Character :public Entity {

	Brain*m_brain;
	BodyBase*m_body;
	Skin*m_skin;

public:
	Character(
		Brain*brain,
		BodyBase*body,
		Skin*skin);
	virtual ~Character();

	virtual void Update(float deltaTime);
	virtual void Draw();
};
