#pragma once
#include<Box2D\Box2D.h>
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"
#include"../PhysicsEngine/BodyBase.h"
class TouchPoint:public BodyBase {
	b2Vec2 m_pos;
	b2Body*m_body;
public:
	TouchPoint(const b2Vec2& pos) :m_pos(pos) {
		b2CircleShape cs;
		cs.m_radius = 10.0f / M2P;
		m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2BodyType::b2_dynamicBody, MaterialType::SENSOR, m_pos);
		m_body->SetUserData(this);
		m_body->SetGravityScale(0);
		m_specifier = ObjectType::OID_TOUCH_POINT;
	}
	void Draw() {
		Locator::GetPrimitiveRenderer()->FillCircle(glm::vec2(m_pos.x, m_pos.y),10);
	}
	//what can I do?
	inline const b2Vec2& GetPos() { return m_pos; }
};