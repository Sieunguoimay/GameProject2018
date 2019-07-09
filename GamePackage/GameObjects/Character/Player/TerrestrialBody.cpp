#include "TerrestrialBody.h"
#include"../../../misc/Assistances.h"
#include"../../../misc/Locator.h"

void TerrestrialBody::SetupBody(glm::vec2 pos,glm::vec2 dimension, b2Body*&body, void*parent)
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
	m_body = body;
	m_body->SetUserData(this);

	m_bodyId = BodyObjectType::BOT_PLAYER;
	id = 2;
	_id = 2;
	//m_leg.Init(body);

	SDL_Log("Player's Mass: %f", body->GetMass());

}

TerrestrialBody::TerrestrialBody()
	:m_onGround(false)
	, m_jumpSpeed(3.50f)
	, m_runSpeed(RUN_SPEED)
{
}

TerrestrialBody::~TerrestrialBody()
{
	m_body = NULL;
}

void TerrestrialBody::Update(float deltaTime)
{
	processCollision();

	//work with normal Ground angle
	if (m_newGroundNormal) {
		//calculate pelvis angle -> costly
		m_normalAngle = glm::degrees(b2Atan2(m_groundNormal.y, m_groundNormal.x));
		if (m_normalAngle< 0)m_normalAngle += 360.0f;
		else if (m_normalAngle >= 360.0f)m_normalAngle -= 360.0f;
		m_newGroundNormal = false;
	}

	//ray cast
	b2Vec2 p1 = m_body->GetPosition();
	b2Vec2 p2 = p1 + b2Vec2(0.0, -2.0f);
	m_minRayCastFraction = 1.0f;//initial
	Locator::GetPhysicsFactory()->GetB2World()->RayCast(this, p1, p2);



	//AABB Query
	//m_pTouchPoint = NULL;
	m_pTouchPoints.clear();
	m_nearestTouchPoint = NULL;
	b2AABB aabb;
	aabb.lowerBound = m_body->GetPosition() - b2Vec2(1.0f, 1.0f);
	aabb.upperBound = m_body->GetPosition() + b2Vec2(1.0f, 1.0f);
	Locator::GetPhysicsFactory()->GetB2World()->QueryAABB(this, aabb);
	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
		aabb.lowerBound.x*M2P, aabb.lowerBound.y*M2P,
		(aabb.upperBound.x - aabb.lowerBound.x)*M2P,
		(aabb.upperBound.y - aabb.lowerBound.y)*M2P), 0);

	//reset
	//if (m_body->GetPosition().y < -500.0f/M2P) {
	//	Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
	//	SetupBody(glm::vec2(0, 100), glm::vec2(80, 160));
	//}


	//update the onGround state
	m_onGround = false;
	float half_height = m_body->GetFixtureList()->GetNext()->GetShape()->m_radius;

	for (b2ContactEdge*contactEdge = m_body->GetContactList(); contactEdge != nullptr; contactEdge = contactEdge->next) {
		
		b2Contact*contact = contactEdge->contact;
		
		if (contact->IsTouching()) {
			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);
			
			//check if the point is below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_body->GetPosition().y - half_height) {
					Locator::GetPrimitiveRenderer()->DrawCircle(M2P*b2Vec2ToGlmVec2(manifold.points[i]), 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
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
	if (glm::abs(vel.x) > 0.0001) {
		if (vel.x > 0) {
			m_dir.Set(1.0f, 0.0f);
		}
		else {
			m_dir.Set(-1.0f, 0.0f);
		}
	}
}


float32 TerrestrialBody::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction)
{
	for (b2Fixture*a = m_body->GetFixtureList(); a; a = a->GetNext())
		if (fixture == a)
			return -1;

	if (fraction < m_minRayCastFraction) {//this is ground fixture
		m_minRayCastFraction = fraction;
		if (m_groundNormal != normal) {
			m_groundNormal = normal;
			m_newGroundNormal = true;
		}
	}

	return fraction;
}

void TerrestrialBody::processCollision()
{
	if (m_contactInfo.hasContact) {
		SDL_Log("Player %d is contacting with %d", m_bodyId,m_contactInfo.other->GetSpecifier());
	}
}

//Handle AABB Query 
bool TerrestrialBody::ReportFixture(b2Fixture * fixture)
{
	void*otherBody = fixture->GetBody()->GetUserData();

	if (otherBody == NULL)return true;

	BodyBase*body = ((BodyBase*)(otherBody));
	if (body == this) return true;

	if (body->GetSpecifier() == BodyObjectType::BOT_TOUCH_POINT) {
		
		m_pTouchPoints.push_back(((TouchPoint*)body));
		b2AABB aabb = fixture->GetAABB(0);
		Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
			aabb.lowerBound.x*M2P, aabb.lowerBound.y*M2P,
			(aabb.upperBound.x - aabb.lowerBound.x)*M2P,
			(aabb.upperBound.y - aabb.lowerBound.y)*M2P), 0);
	}
	return true;
}

TouchPoint*TerrestrialBody::GetNearestTouchPoint(const b2Vec2& point) {
	if (m_pTouchPoints.empty())return NULL;

	float minDisSqr = std::numeric_limits<float>::max();
	for (auto&a : m_pTouchPoints) {
		float disSqr = (a->GetPos() - point).LengthSquared();
		if (minDisSqr > disSqr) {
			minDisSqr = disSqr;
			m_nearestTouchPoint = a;
		}
	}

	return m_nearestTouchPoint;
}

void TerrestrialBody::Stop()
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	if (m_onGround) m_body->SetLinearVelocity(b2Vec2(vel.x*0.85, vel.y));
	else m_body->SetLinearVelocity(b2Vec2(vel.x, vel.y));

}

void TerrestrialBody::Move(bool left_right, float speed)
{
	m_runSpeed = speed;
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

void TerrestrialBody::HoldTouchPoint(const b2Vec2& point)
{
	if (m_nearestTouchPoint != NULL&&m_joint==NULL) {
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_body, m_nearestTouchPoint->GetBody(), 
			m_body->GetWorldCenter(), m_nearestTouchPoint->GetBody()->GetWorldCenter());
		m_joint = (b2DistanceJoint*)Locator::GetPhysicsFactory()->GetB2World()->CreateJoint(&jointDef);
	}
}

void TerrestrialBody::LeaveTouchPoint()
{
	if (m_joint) {
		Locator::GetPhysicsFactory()->GetB2World()->DestroyJoint(m_joint);
		m_joint = NULL;
	}
}
