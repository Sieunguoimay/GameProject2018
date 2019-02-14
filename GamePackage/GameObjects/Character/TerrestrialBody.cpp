#include "TerrestrialBody.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"

void TerrestrialBody::setupBody(glm::vec2 pos,glm::vec2 dimension)
{
	//Create the circles
	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_dynamicBody, MEAT, b2Vec2(pos.x / M2P, pos.y / M2P));

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / (2.0f*M2P));
	m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MEAT));

	//sensor shape
	//cs.m_radius = (dimension.x+10.0f) / M2P;
	//cs.m_p.Set(0.0f, 0.0f);
	//m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(SENSOR,&cs));

	m_body->SetFixedRotation(true);
	m_body->SetUserData(this);
	SDL_Log("Player's Mass: %f", m_body->GetMass());
}

TerrestrialBody::TerrestrialBody(glm::vec2 pos, glm::vec2 dimension)
	:BodyBase(pos)
	, m_onGround(false)
	, m_jumpSpeed(5.0f)
	, m_runSpeed(3.0f)
{
	setupBody(pos, dimension);
	m_leg.Init(m_body);
}

TerrestrialBody::~TerrestrialBody()
{
}

void TerrestrialBody::Update(float deltaTime)
{
	//reset
	if (m_pos.y < -500.0f) {
		Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
		setupBody(glm::vec2(0, 100), glm::vec2(80, 160));
	}


	//update the onGround state
	m_onGround = false;
	float half_height = m_body->GetFixtureList()->GetNext()->GetShape()->m_radius / 2.0f;
	for (b2ContactEdge*contactEdge = m_body->GetContactList();
		contactEdge != nullptr; contactEdge = contactEdge->next) {

		b2Contact*contact = contactEdge->contact;
		if (contact->IsTouching()) {
			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);
			//check if the point is below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_body->GetPosition().y - half_height) {
					m_onGround = true;
					break;
				}
			}
		}
	}


	//limit the speed
	b2Vec2 vel = m_body->GetLinearVelocity();
	if (glm::abs(vel.x) > m_runSpeed) {
		float velX = (vel.x / glm::abs(vel.x))*m_runSpeed;
		float impulse = m_body->GetMass()*(velX - vel.x);
		//impulse /= 6.0f;
		m_body->ApplyLinearImpulse(b2Vec2(impulse, 0), m_body->GetWorldCenter(), true);
	}
	if (glm::abs(vel.y) > m_jumpSpeed) {
		float velY = (vel.y / glm::abs(vel.y))*m_jumpSpeed;
		float impulse = m_body->GetMass()*(velY - vel.y);
		//impulse /= 6.0f;
		m_body->ApplyLinearImpulse(b2Vec2(0, impulse), m_body->GetWorldCenter(), true);
	}

	m_pos = glm::vec2(M2P*m_body->GetPosition().x, M2P*m_body->GetPosition().y);

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
	b2Vec2 vel = m_body->GetLinearVelocity();
	if (m_onGround) m_body->SetLinearVelocity(b2Vec2(vel.x*0.85, vel.y));
	else m_body->SetLinearVelocity(b2Vec2(vel.x, vel.y));

}

void TerrestrialBody::Run(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	float impulse = runDir*m_body->GetMass()*m_runSpeed;
	m_body->ApplyLinearImpulse(b2Vec2(impulse, 0), m_body->GetWorldCenter(), true);

}

void TerrestrialBody::Jump()
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	m_body->SetLinearVelocity(vel);
}

void TerrestrialBody::JumpAndRun(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	vel.x = runDir*m_runSpeed;
	m_body->SetLinearVelocity(vel);

}

bool TerrestrialBody::IsOnGround()
{
	return m_onGround;
}

