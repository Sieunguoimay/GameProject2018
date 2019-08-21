#pragma once
#include"Entity.h"
#include"../Skin.h"
#include"../PhysicsEngine/BodyDragAndDrop.h"
#include"../PhysicsEngine/BodyBase.h"
//Don't inherit this class (though doing so is harmless)
//Inherit the 2 children of it.

class HavingBodyEntity :public AABBEntity ,public BodyBase{
protected:
	//one object can have different skin, while body and brain define the object
	BodyDragAndDrop m_bodyDragAndDrop;
	HavingBodyEntity(/*ObjectType type*/);
public:
	virtual ~HavingBodyEntity();

	inline virtual void Init()override { SetupBody(); }

	//must implement
	virtual void SetupBody(bool atRunTime = false) = 0;
	//void HandleBeginContact(b2Contact*contact, b2Fixture*fixture)override {}
	//void HandleEndContact(b2Contact*contact, b2Fixture*fixture) override {}

	//optional from the higher level
	void OnSelect(const glm::vec4&AABB)override;
	void OnRelease(const glm::vec4&AABB)override;
	void ApplyAABB(const glm::vec4&AABB)override;
};


//these following classes are transparent for the purpose of skins which
//are either Animation or NoAnimation
//so that the lower level don't have to mind about 
//which kind of skin it's using.

class AnimationBodyEntity :public HavingBodyEntity {
	int m_animationIndexBuffer;
protected:
	//this is just a pointer pointing to the skin object
	//no deleting.
	AnimationSkin*m_skin;
public:
	AnimationBodyEntity(AnimationSkin*skin/*, ObjectType type*/,glm::vec4 AABB);
	virtual ~AnimationBodyEntity();

	//animation skin requires updating
	virtual void Update(float deltaTime)override;
	virtual void Draw()override;
	inline glm::vec4 CalculateAABB() override { return m_skin->GetAABB(); };
	//optional from the higher level
	void OnSelect(const glm::vec4&AABB)override;
	void OnRelease(const glm::vec4&AABB)override;
	void ApplyAABB(const glm::vec4&AABB)override;
	
	inline AnimationSkin*GetSkin() { return m_skin; }
};



class NoAnimationBodyEntity :public HavingBodyEntity {
	glm::vec2 m_AABBDimensionBuffer;
	glm::vec2 m_sizeBuffer; 
protected:
	//this is just a pointer pointing to the skin object
	//no deleting.
	NoAnimationSkin*m_skin;
public:
	NoAnimationBodyEntity(NoAnimationSkin*skin/*, ObjectType*/,glm::vec4 AABB);
	virtual ~NoAnimationBodyEntity();
	inline glm::vec4 CalculateAABB() override { return m_skin->GetAABB(); }
	virtual void Update(float deltaTime)override;
	virtual void Draw()override;
	//optional from the higher level
	void OnSelect(const glm::vec4&AABB)override;
	void ApplyAABB(const glm::vec4&AABB)override;

	inline NoAnimationSkin*GetSkin() { return m_skin; }
};
