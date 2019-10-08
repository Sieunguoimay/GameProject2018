#pragma once
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"
#include"../PhysicsEngine/BodyBase.h"
class TouchPoint:public BodyBase {
	BodyBase*m_pOwner;
public:
	void SetupBody(const b2Vec2& pos,BodyBase*pOwner);
	BodyBase*GetOwner() { return m_pOwner; }
};

inline void TouchPoint::SetupBody(const b2Vec2& pos, BodyBase*pOwner) {
	m_pOwner = pOwner;
	b2CircleShape cs;
	cs.m_radius = 10.0f / M2P;
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2BodyType::b2_dynamicBody, MaterialType::SENSOR, pos);
	m_body->SetUserData(this);
	m_body->SetGravityScale(0);
	m_bodyId = BodyObjectType::BOT_TOUCH_POINT;
}
