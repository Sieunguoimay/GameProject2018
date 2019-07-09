#include "MovableBox.h"

MovableBox::MovableBox(NoAnimationSkin * skin, glm::vec4 AABB)
	:NoAnimationBodyEntity(skin, AABB)
{
}

void MovableBox::SetupBody(bool atRunTime)
{
	b2Vec2 pos = 0.5f/ M2P*b2Vec2(m_AABB.x + m_AABB.z, m_AABB.y + m_AABB.w);
	float width = 0.5f / M2P*(m_AABB.z - m_AABB.x);
	float height = 0.5f / M2P
		*(m_AABB.w - m_AABB.y);

	b2PolygonShape ps;
	ps.SetAsBox(width, height);
	m_body = Locator::GetPhysicsFactory()->CreateBody(&ps, b2BodyType::b2_dynamicBody, MaterialType::STEEL, pos);
	m_body->SetUserData(this);
	m_bodyId = BodyObjectType::BOT_ROCK;
	id = 4;
	_id = 4;

	m_touchPoints[0].SetPos(pos + 0.95f*b2Vec2(ps.m_vertices[0].x,0));
	m_touchPoints[1].SetPos(pos + 0.95f*b2Vec2(0,ps.m_vertices[1].y));
	m_touchPoints[2].SetPos(pos + 0.95f*b2Vec2(ps.m_vertices[2].x,0));
	m_touchPoints[3].SetPos(pos + 0.95f*b2Vec2(0,ps.m_vertices[3].y));

	for (auto&touchPoint : m_touchPoints) {
		touchPoint.Init();
		b2RevoluteJointDef jointDef;
		jointDef.Initialize(m_body, touchPoint.GetBody(), touchPoint.GetBody()->GetWorldCenter());
		Locator::GetPhysicsFactory()->GetB2World()->CreateJoint(&jointDef);
	}
}
