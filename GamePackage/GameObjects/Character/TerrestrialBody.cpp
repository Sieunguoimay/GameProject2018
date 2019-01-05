#include "TerrestrialBody.h"
#include"../../misc/Assistances.h"

TerrestrialBody::TerrestrialBody(b2World * world, glm::vec2 pos, glm::vec2 dimension)
	:BodyBase(world,pos,dimension)
	, m_onGround(false)
	, m_jumpSpeed(5.0f)
	, m_runSpeed(3.0f)

{
		
}

TerrestrialBody::~TerrestrialBody()
{
}

void TerrestrialBody::Update(float deltaTime)
{
	//update the onGround state
	m_onGround = false;
	float half_height = m_bodyKit.GetDimension().y / (4.0f*M2P);
	for (b2ContactEdge*contactEdge = m_bodyKit.GetBody()->GetContactList(); 
		contactEdge != nullptr; contactEdge = contactEdge->next) {

		b2Contact*contact = contactEdge->contact;
		if (contact->IsTouching()) {
			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);
			//check if the point is below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_bodyKit.GetBody()->GetPosition().y - half_height) {
					m_onGround = true;
					break;
				}
			}
		}
	}


	//limit the speed
	b2Vec2 vel = m_bodyKit.GetBody()->GetLinearVelocity();
	if (glm::abs(vel.x) > m_runSpeed) {
		float velX = (vel.x / glm::abs(vel.x))*m_runSpeed;
		float impulse = m_bodyKit.GetBody()->GetMass()*(velX - vel.x);
		//impulse /= 6.0f;
		m_bodyKit.GetBody()->ApplyLinearImpulse(b2Vec2(impulse, 0), m_bodyKit.GetBody()->GetWorldCenter(), true);
	}
	if (glm::abs(vel.y) > m_jumpSpeed) {
		float velY = (vel.y / glm::abs(vel.y))*m_jumpSpeed;
		float impulse = m_bodyKit.GetBody()->GetMass()*(velY - vel.y);
		//impulse /= 6.0f;
		m_bodyKit.GetBody()->ApplyLinearImpulse(b2Vec2(0, impulse), m_bodyKit.GetBody()->GetWorldCenter(), true);
	}

}


void TerrestrialBody::Stop()
{
	b2Vec2 vel = m_bodyKit.GetBody()->GetLinearVelocity();
	if (m_onGround) m_bodyKit.GetBody()->SetLinearVelocity(b2Vec2(vel.x*0.85, vel.y));
	else m_bodyKit.GetBody()->SetLinearVelocity(b2Vec2(vel.x, vel.y));

}

void TerrestrialBody::Run(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	float impulse = runDir*m_bodyKit.GetBody()->GetMass()*m_runSpeed;
	m_bodyKit.GetBody()->ApplyLinearImpulse(b2Vec2(impulse, 0), m_bodyKit.GetBody()->GetWorldCenter(), true);

}

void TerrestrialBody::Jump()
{
	b2Vec2 vel = m_bodyKit.GetBody()->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	m_bodyKit.GetBody()->SetLinearVelocity(vel);
}

void TerrestrialBody::JumpAndRun(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	b2Vec2 vel = m_bodyKit.GetBody()->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	vel.x = runDir*m_runSpeed;
	m_bodyKit.GetBody()->SetLinearVelocity(vel);

}

bool TerrestrialBody::IsOnGround()
{
	return m_onGround;
}
