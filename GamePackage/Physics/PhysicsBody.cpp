#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(b2World*pWorld, b2Body*body)
	:m_pWorld(pWorld),m_userData(NULL),m_body(body)
{
	m_ratio = 0.0f;
	m_userData = new UserData();
	m_userData->pointer = NULL;
	m_userData->type = DEFAULT_OBJECT_TYPE;
	m_body->SetUserData(m_userData);
}

PhysicsBody::~PhysicsBody()
{
	m_userData->pointer = NULL;
	delete m_userData;
	m_pWorld->DestroyBody(m_body);
	m_pWorld = NULL;
}
