#pragma once
#include"../2D/Sprite.h"
#include"../2D/Animation.h"
#include"../Physics/PhysicsBody.h"
#include"../2D/AnimatedSprite.h"
class BaseObject {
protected:
	AnimatedSprite*m_animatedSprite;
	int m_width;
	int m_height;

	float m_angle;
	b2Vec2 m_pos;
	float m_boundRadius;
public:
	BaseObject(Sprite*sprite,float x, float y,int w, int h);
	virtual ~BaseObject();

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	//Getters and setters;

	virtual void SetSize(int width, int height) { this->m_width = width; this->m_height = height; }
	virtual void SetAngle(float angle) { this->m_angle = angle; }
	virtual void SetPos(b2Vec2 pos) { this->m_pos = pos; }
	virtual void SetPos(float x,float y) { this->m_pos.Set(x,y); }

	virtual AnimatedSprite*GetAnimatedSprite() { return this->m_animatedSprite; }
	virtual b2Vec2 GetPos() { return this->m_pos; }
	virtual float GetAngle() { return this->m_angle; }
	virtual int GetWidth() { return this->m_width; }
	virtual int GetHeight() { return this->m_height; }

	const float& GetBoundRadius() const { return this->m_boundRadius; }
};

class NoPhysicsObject :public BaseObject{

public:
	NoPhysicsObject(Sprite*sprite, float x, float y, int w, int h);
	virtual ~NoPhysicsObject();
	virtual void Update(float deltaTime);
	virtual void Draw();
};

class PhysicsObject :public BaseObject{
protected:
	PhysicsBody*m_physicsBody;
public:
	PhysicsObject(Sprite*sprite, float x, float y, int w, int h);//pass in something that hardly changes
	virtual ~PhysicsObject();

	virtual void ProcessCollision(UserData*other)=0;

	virtual void Update(float deltaTime);
	virtual void Draw();

	PhysicsBody*GetPhysicsBody() { return m_physicsBody; }
	void SetPhysicsBody(PhysicsBody*physicsBody) { this->m_physicsBody = physicsBody; }

	//override
	void SetPos(float x, float y) { m_physicsBody->SetPos(b2Vec2(x,y)); }

	enum {
		CIRCLE_SHAPE,
		BOX_SHAPE
	};
};

class StandStillObject:public PhysicsObject {
public:

	StandStillObject(Sprite*sprite, float x, float y, int w, int h,int shape);
	~StandStillObject();

	virtual void ProcessCollision(UserData*other);

	virtual void Update(float deltaTime);
	virtual void Draw();

};

