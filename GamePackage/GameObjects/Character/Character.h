#pragma once
#include"Box2D\Box2D.h"
#include"../EntityHierachy/HavingBodyEntity.h"
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
	Brain(BodyBase*bodyBase, AnimationSkin*skin):m_pBodyBase(bodyBase),m_pSkin(skin){}
	inline virtual ~Brain(){}
	virtual void Update(float deltaTime) = 0;
	inline AnimationSkin*GetSkin() { return m_pSkin; }
	inline BodyBase*GetBodyBase() { return m_pBodyBase; }
protected:	
	AnimationSkin*m_pSkin;
	BodyBase*m_pBodyBase;
};




class Character :public AnimationBodyEntity {
protected:
	Brain*m_brain;			//take in the bodyBase for controlling and Skin for animation change	
	BodyBase*m_bodyBase;	//take in the b2Body* of this entity
public:
	Character(AnimationSkin*animationSkin,const glm::vec4&AABB);
	virtual ~Character();
	virtual void Update(float deltaTime);

	inline void AttachBrain(Brain*brain) { m_brain = brain; }
	inline void AttachBodyBase(BodyBase*bodyBase) { m_bodyBase = bodyBase; }
};
