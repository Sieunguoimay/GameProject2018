#include "TerrestrialBody.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"

void TerrestrialBody::SetupBody(glm::vec2 pos,glm::vec2 dimension, b2Body*&body)
{
	//Create the circles
	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_dynamicBody, MEAT, b2Vec2(pos.x / M2P, pos.y / M2P));

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / (2.0f*M2P));
	body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MEAT));

	//sensor shape
	//cs.m_radius = (dimension.x+10.0f) / M2P;
	//cs.m_p.Set(0.0f, 0.0f);
	//m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(SENSOR,&cs));

	body->SetFixedRotation(true);
	body->SetUserData(this);
	m_pBody = body;

	SDL_Log("Player's Mass: %f", body->GetMass());
}

TerrestrialBody::TerrestrialBody()
	:m_onGround(false)
	, m_jumpSpeed(5.0f)
	, m_runSpeed(3.0f)
	,m_pBody(NULL)
{
}

TerrestrialBody::~TerrestrialBody()
{
	m_pBody = NULL;
}

void TerrestrialBody::Update(float deltaTime)
{
	//reset
	//if (m_pBody->GetPosition().y < -500.0f/M2P) {
	//	Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_pBody);
	//	SetupBody(glm::vec2(0, 100), glm::vec2(80, 160));
	//}


	//update the onGround state
	m_onGround = false;
	float half_height = m_pBody->GetFixtureList()->GetNext()->GetShape()->m_radius / 2.0f;
	for (b2ContactEdge*contactEdge = m_pBody->GetContactList();
		contactEdge != nullptr; contactEdge = contactEdge->next) {

		b2Contact*contact = contactEdge->contact;
		if (contact->IsTouching()) {
			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);
			//check if the point is below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_pBody->GetPosition().y - half_height) {
					m_onGround = true;
					break;
				}
			}
		}
	}


	//limit the speed
	b2Vec2 vel = m_pBody->GetLinearVelocity();
	if (glm::abs(vel.x) > m_runSpeed) {
		float velX = (vel.x / glm::abs(vel.x))*m_runSpeed;
		float impulse = m_pBody->GetMass()*(velX - vel.x);
		//impulse /= 6.0f;
		m_pBody->ApplyLinearImpulse(b2Vec2(impulse, 0), m_pBody->GetWorldCenter(), true);
	}
	if (glm::abs(vel.y) > m_jumpSpeed) {
		float velY = (vel.y / glm::abs(vel.y))*m_jumpSpeed;
		float impulse = m_pBody->GetMass()*(velY - vel.y);
		//impulse /= 6.0f;
		m_pBody->ApplyLinearImpulse(b2Vec2(0, impulse), m_pBody->GetWorldCenter(), true);
	}

	//debug draw

	m_leg.Update(deltaTime);
}

void TerrestrialBody::HandleBeginContact(b2Contact * contact, b2Fixture*fixture)
{
	//contact->GetWorldManifold(&m_manifold);
	//if (((BodyBase*)fixture->GetBody()->GetUserData())->GetSpecifier() == BodySpecifier::GROUND)
	//	m_groundFixture = fixture;

}


void TerrestrialBody::Stop()
{
	b2Vec2 vel = m_pBody->GetLinearVelocity();
	if (m_onGround) m_pBody->SetLinearVelocity(b2Vec2(vel.x*0.85, vel.y));
	else m_pBody->SetLinearVelocity(b2Vec2(vel.x, vel.y));

}

void TerrestrialBody::Run(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	float impulse = runDir*m_pBody->GetMass()*m_runSpeed;
	m_pBody->ApplyLinearImpulse(b2Vec2(impulse, 0), m_pBody->GetWorldCenter(), true);

}

void TerrestrialBody::Jump()
{
	b2Vec2 vel = m_pBody->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	m_pBody->SetLinearVelocity(vel);
}

void TerrestrialBody::JumpAndRun(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	b2Vec2 vel = m_pBody->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	vel.x = runDir*m_runSpeed;
	m_pBody->SetLinearVelocity(vel);

}

bool TerrestrialBody::IsOnGround()
{
	return m_onGround;
}

