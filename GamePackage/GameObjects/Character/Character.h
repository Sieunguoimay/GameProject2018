#pragma once
#include"Box2D\Box2D.h"
#include"../EntityHierachy/Entity.h"
#include"../SpriterEntity.h"
#include"../PhysicsEngine/BodyBase.h"
#include"../Skin.h"





//why AnimationSkin???
//-> because the m_spriterEntity is unque for that specific ScmlEntity
//and it does not keep the information about position or other setup

//for example, when you want to pass the position from body into the spriter
//at update. how can you do that without a memory in between to transfer
//that data to the draw function...


class Brain {

public:
	Brain(BodyBase*body, AnimationSkin*AnimationSkin);
	virtual ~Brain(){}
	virtual void Update(float deltaTime);

	
	
	AnimationSkin*GetSkin() { return m_pSkin; }
	BodyBase*GetBody() { return m_pBody; }
protected:	
	BodyBase*m_pBody;
	AnimationSkin*m_pSkin;
};




class Character :public Entity {

	Brain*m_brain;
	BodyBase*m_body;
	AnimationSkin*m_skin;

public:
	Character(
		Brain*brain,
		BodyBase*body,
		AnimationSkin*AnimationSkin);
	virtual ~Character();

	virtual void Update(float deltaTime);
	virtual void Draw();
};
