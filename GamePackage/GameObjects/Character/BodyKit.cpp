#include"BodyKit.h"
#include"../../misc/Assistances.h"

void BodyKit::SetupBody(glm::vec2 pos, glm::vec2 dimension)
{
	m_dimension = dimension;
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x / M2P, pos.y / M2P);//Example: if x is 100 then position.x is 1.0f
	bd.fixedRotation = true;
	m_body = m_pWorld->CreateBody(&bd);



	//Create the circles
	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	b2FixtureDef fd;
	fd.shape = &cs;
	fd.density = 1;
	fd.friction = 0.5;
	fd.restitution = 0.15f;
	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));

	m_fixtures.push_back(m_body->CreateFixture(&fd));

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / (2.0f*M2P));
	m_fixtures.push_back(m_body->CreateFixture(&fd));
}

BodyKit::BodyKit(b2World*world, glm::vec2 pos, glm::vec2 dimension)
	:m_pWorld(world), m_dimension(dimension)

{
	SetupBody(pos, dimension);
}

BodyKit::~BodyKit()
{
	m_pWorld->DestroyBody(m_body);
	m_fixtures.clear();
}

void BodyKit::SetLinearVelocity(float x, float y)
{
	m_body->SetLinearVelocity(b2Vec2(x, y));
}

void BodyKit::ApplyForce(float x, float y)
{
	m_body->ApplyForce(b2Vec2(x, y), m_body->GetWorldCenter(), true);
}

void BodyKit::SendInfo(UserData userData)
{
	m_collisions.insert(std::pair<void*, EntityType>(userData.m_pointer, userData.m_type));
}

void BodyKit::ClearCollisionData()
{
	m_collisions.clear();
}

glm::vec2 BodyKit::GetPosition()
{
	b2Vec2 pos = M2P*m_body->GetPosition();
	return glm::vec2(pos.x, pos.y);
}


float BodyKit::GetAngle()
{
	return m_body->GetAngle();
}

CollisionData & BodyKit::GetAllCollisions()
{
	// TODO: insert return statement here
	return m_collisions;
}

b2Body * BodyKit::GetBody()
{
	return m_body;
}

glm::vec2 BodyKit::GetDimension()
{
	return m_dimension;
}
