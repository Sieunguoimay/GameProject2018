#pragma once
#include"Entity.h"
#include"../Skin.h"
#include"BodyProtocol.h"

//Don't inherit this class (though doing so is harmless)
//Inherit the 2 children of it.
class HavingBodyEntity :public AABBEntity,public BodyProtocol{
protected:
	//one object can have different skin, while body and brain define the object
	Skin*m_skinBase;
	b2Body*m_body;
	HavingBodyEntity(Skin*skin, ObjectType type);
public:
	virtual ~HavingBodyEntity();

	//must implement
	virtual void SetupBody() = 0;

	//optional from the higher level
	void OnSelect(const glm::vec2&AA, const glm::vec2&BB)override;
	void OnRelease(const glm::vec2&AA, const glm::vec2&BB)override;

	//update is a must.
	virtual void Update(float deltaTime)override;
	//no need to implement this.
	void Draw()override;
};


//these following classes are transparent for the purpose of skins which
//are either Animation or NoAnimation
//so that the lower level don't have to mind about 
//which kind of skin it's using.

class AnimationBodyEntity :public HavingBodyEntity {
protected:
	//this is just a pointer pointing to the skin object
	//no deleting.
	AnimationSkin*m_skin;
public:
	AnimationBodyEntity(AnimationSkin*skin, ObjectType type);
	virtual ~AnimationBodyEntity();

	//animation skin requires updating
	virtual void Update(float deltaTime)override;
};



class NoAnimationBodyEntity :public HavingBodyEntity {
protected:
	//this is just a pointer pointing to the skin object
	//no deleting.
	NoAnimationSkin*m_skin;
public:
	NoAnimationBodyEntity(NoAnimationSkin*skin, ObjectType);
	virtual ~NoAnimationBodyEntity();
};
