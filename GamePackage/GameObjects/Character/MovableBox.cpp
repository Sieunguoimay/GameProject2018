#include "MovableBox.h"

MovableBox::MovableBox(NoAnimationSkin * skin, glm::vec4 AABB)
	:NoAnimationBodyEntity(skin, AABB)
{}

void MovableBox::SetupBody(bool atRunTime)
{
	b2Vec2 pos = 0.5f/ M2P*b2Vec2(m_AABB.x + m_AABB.z, m_AABB.y + m_AABB.w);
	float width = 0.45f / M2P*(m_AABB.z - m_AABB.x);
	float height = 0.45f / M2P
		*(m_AABB.w - m_AABB.y);

	b2PolygonShape ps;
	ps.SetAsBox(width, height);
	m_body = Locator::GetPhysicsFactory()->CreateBody(&ps, b2BodyType::b2_dynamicBody, MaterialType::STEEL, pos);
	m_body->SetUserData(m_bodyBaseThis);
	m_bodyId = BodyObjectType::BOT_ROCK;

	m_touchPoints[0].SetupBody(pos + 0.95f*b2Vec2(ps.m_vertices[0].x,0),this);
	m_touchPoints[1].SetupBody(pos + 0.95f*b2Vec2(0,ps.m_vertices[1].y), this);
	m_touchPoints[2].SetupBody(pos + 0.95f*b2Vec2(ps.m_vertices[2].x,0), this);
	m_touchPoints[3].SetupBody(pos + 0.95f*b2Vec2(0,ps.m_vertices[3].y), this);

	for (auto&touchPoint : m_touchPoints) {
		b2RevoluteJointDef jointDef;
		jointDef.Initialize(m_body, touchPoint.GetBody(), touchPoint.GetBody()->GetWorldCenter());
		Locator::GetPhysicsFactory()->GetB2World()->CreateJoint(&jointDef);
	}
}
