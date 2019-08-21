#pragma once
#include<Box2D\Box2D.h>
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"
#include"../PhysicsEngine/BodyBase.h"
class TouchPoint:public BodyBase {
	b2Vec2 m_pos;
public:
	void Init() {
		b2CircleShape cs;
		cs.m_radius = 10.0f / M2P;
		m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2BodyType::b2_dynamicBody, MaterialType::SENSOR, m_pos);
		m_body->SetUserData(this);
		m_body->SetGravityScale(0);
		m_bodyId = BodyObjectType::BOT_TOUCH_POINT;


	}
	void Draw() { Locator::GetPrimitiveRenderer()->DrawCircle(Utils::b2ToGlm(M2P*m_pos), 5, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); }
	//what can I do?
	inline const b2Vec2& GetPos() { m_pos = m_body->GetPosition(); return m_pos; }
	inline void SetPos(const b2Vec2&pos) { m_pos = pos; }
};
