#pragma once
#include"Box2D\Box2D.h"
struct UserData {
	int type;
	void*pointer;
};
enum ObjectType{
	DEFAULT_OBJECT_TYPE,
	SOMETHING,
};

class PhysicsBody {
	UserData*m_userData;
	b2Body*m_body;
	b2World*m_pWorld;
	float m_ratio;
public:
	PhysicsBody(b2World*pWorld,b2Body*body);
	~PhysicsBody();
	//initializer
	void SetUserData(int type, void* pointer ){ m_userData->type = type;  m_userData->pointer = pointer; }
	void SetBody(b2Body*body) { this->m_body = body; }
	void SetRatio(float ratio) { this->m_ratio = ratio; }

	//public using
	b2Vec2 GetPos() { return m_ratio*this->m_body->GetPosition(); }
	b2Vec2 GetVel() { return m_ratio*this->m_body->GetLinearVelocity(); }
	float GetAngle() { return this->m_body->GetAngle(); }
	b2Body*GetBody() { return this->m_body; }
	UserData*GetUserData() { return this->m_userData; }

	void SetPos(b2Vec2 pos) { this->m_body->SetTransform((1.0f/ m_ratio)*pos, GetAngle()); }
};
