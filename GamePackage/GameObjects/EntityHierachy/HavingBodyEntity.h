#pragma once
#include"Entity.h"
#include"../Skin.h"
#include"BodyProtocol.h"
#include"../PhysicsEngine/BodyDragAndDrop.h"
//Don't inherit this class (though doing so is harmless)
//Inherit the 2 children of it.
class HavingBodyEntity :public AABBEntity,public BodyProtocol,public BodyDragAndDrop{
protected:
	//one object can have different skin, while body and brain define the object
	Skin*m_skinBase;
	b2Body*m_body;

	HavingBodyEntity(Skin*skin, ObjectType type);

public:
	virtual ~HavingBodyEntity();

	inline virtual void Init()override { SetupBody(); }
	//must implement
	virtual void SetupBody(bool atRunTime = false) = 0;

	//optional from the higher level
	void OnSelect(const glm::vec4&AABB)override;
	void OnRelease(const glm::vec4&AABB)override;
	void ApplyAABB(const glm::vec4&AABB)override;

	//update is a must.
	virtual void Update(float deltaTime)override;
	//no need to implement this.
	void Draw()override;

	inline b2Body*GetBody() { return m_body; }
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
	AnimationBodyEntity(AnimationSkin*skin, ObjectType type,glm::vec4 AABB);
	virtual ~AnimationBodyEntity();

	//animation skin requires updating
	virtual void Update(float deltaTime)override;
	inline const glm::vec4& CalculateAABB() override { return m_skin->GetAABB(); };
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
	NoAnimationBodyEntity(NoAnimationSkin*skin, ObjectType,glm::vec4 AABB);
	virtual ~NoAnimationBodyEntity();
	const glm::vec4& CalculateAABB() override { return m_skin->GetAABB(); }

	//optional from the higher level
	void OnSelect(const glm::vec4&AABB)override;
	void ApplyAABB(const glm::vec4&AABB)override;

	inline NoAnimationSkin*GetSkin() { return m_skin; }
};
