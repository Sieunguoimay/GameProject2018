#include "PlayerSensor.h"
#include"Player.h"
#include"../../../misc/Math/Geometry.h"
#include"PlayerSkin.h"

PlayerSensor::PlayerSensor(Player * pOwner)
	: m_pOwner(pOwner)
{
	m_rayCastStartPoint = b2Vec2(0,0);//center in b2Body
	m_rayCastEndPoint = b2Vec2(0, -1.0f);
	SDL_Log("PlayerSensor created");
}
PlayerSensor::~PlayerSensor()
{
	SDL_Log("PlayerSensor deleted");
}

void PlayerSensor::Update(float deltaTime)
{
	m_pOwner->m_memory.ClearAllPercepts();
	m_perceptClosestRock.m_pBody = NULL;

	updateRayCast();
	updateAABBRadar();
	processContacts();

	if (m_perceptClosestRock.m_pBody != NULL) 
		m_pOwner->m_memory.AddPercept(m_perceptClosestRock,PPT_CLOSEST_ROCK);
}

void PlayerSensor::updateAABBRadar()
{

	m_iterator = m_terrain.first();

	if (m_pOwner->GetSpriterEntity()->GetFlip() == FlipType::FLIP_NONE) {
		m_radarAABB.lowerBound = m_pOwner->GetBody()->GetPosition() - b2Vec2(0.0f, 1.0f);
		m_radarAABB.upperBound = m_pOwner->GetBody()->GetPosition() + b2Vec2(1.0f, 1.0f);
	}
	else if (m_pOwner->GetSpriterEntity()->GetFlip() == FlipType::HORIZONTAL_FLIP) {
		m_radarAABB.lowerBound = m_pOwner->GetBody()->GetPosition() - b2Vec2(1.0f, 1.0f);
		m_radarAABB.upperBound = m_pOwner->GetBody()->GetPosition() + b2Vec2(0.0f, 1.0f);
	}
	Locator::GetPhysicsFactory()->GetB2World()->QueryAABB(this, m_radarAABB);


	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
		m_radarAABB.lowerBound.x*M2P, m_radarAABB.lowerBound.y*M2P,
		(m_radarAABB.upperBound.x - m_radarAABB.lowerBound.x)*M2P,
		(m_radarAABB.upperBound.y - m_radarAABB.lowerBound.y)*M2P),
		0, glm::vec2(0, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

//Handle AABB Query 
bool PlayerSensor::ReportFixture(b2Fixture * fixture)
{
	void*rawBody = fixture->GetBody()->GetUserData();
	if (rawBody == NULL)return true;
	BodyBase*body = (BodyBase*)(rawBody);
	if (body->GetBody() == m_pOwner->GetBody()) return true;


	b2AABB aabb = fixture->GetAABB(0);
	const b2Vec2&pos1 = m_pOwner->GetBody()->GetPosition();
	const b2Vec2&pos2 = fixture->GetBody()->GetPosition();
	int what = body->GetSpecifier();
	
	if (what == BodyObjectType::BOT_TOUCH_POINT) {
		Percept* touchPoint = m_pOwner->m_memory.GetPercept(PPT_CLOSEST_TOUCH_POINT);
		bool flag = (touchPoint == NULL);
		if (!flag) {
			float disSqr1 = (touchPoint->m_pBody->GetPosition() - pos1).LengthSquared();
			float disSqr2 = (pos2 - pos1).LengthSquared();
			if (disSqr2 < disSqr1)flag = true;
		}
		if (flag) 
			m_pOwner->m_memory.AddPercept(Percept(fixture->GetBody(), PPT_CLOSEST_TOUCH_POINT, SM_AABB_QUERY),PPT_CLOSEST_TOUCH_POINT);

	}

	if (what == BodyObjectType::BOT_ROCK) {
		//add closest Rock to percept
		bool flag1 = (m_pOwner->m_memory.GetPercept(PPT_CLOSEST_ROCK) == NULL);
		if (!flag1) {
			auto aabb1 = Utils::getAABB(m_pOwner->m_memory.GetPercept(PPT_CLOSEST_ROCK)->m_pBody);
			auto p1 = 0.5f*(aabb1.lowerBound + aabb1.upperBound);
			auto p2 = 0.5f*(aabb.lowerBound + aabb.upperBound);
			if ((p2 - pos1).LengthSquared() < (p1 - pos1).LengthSquared()) flag1 = true;
		}
		if (flag1) {
			auto diff = pos1- pos2;
			m_perceptClosestRock = Percept(fixture->GetBody(), 
				PlayerPerceptType::PPT_CLOSEST_ROCK, 
				SensingMechanism::SM_AABB_QUERY, 
				(diff.x > 0 ?
					(diff.x > diff.y ?
					PlayerMemory::ClosestRockFlag::BODY_STATE_NEXT_TO_ROCK_LHS :
					PlayerMemory::ClosestRockFlag::BODY_STATE_ON_ROCK_TOP_RIGHT) 
					:
					(std::abs(diff.x) > diff.y ?
						PlayerMemory::ClosestRockFlag::BODY_STATE_NEXT_TO_ROCK_RHS :
						PlayerMemory::ClosestRockFlag::BODY_STATE_ON_ROCK_TOP_LEFT)));
		}


		//Geometry::clipPolygonAgainstAABB(
		//	((b2PolygonShape*)fixture->GetShape()),m_radarAABB,
		//	[this](const b2Vec2&vertex) {addVertexToContainer(vertex); },
		//	[fixture](const b2Vec2&localVertex) {return fixture->GetBody()->GetWorldPoint(localVertex); });
		//make sure that when it is rock, we only process the first fixture, but we all know that rock has only one fixture
		//return false;
	}

	return true;
}

//void PlayerSensor::calculateNearestTouchPoint()
//{
	//b2Vec2 point = 1.0f / M2P * Utils::glmToB2(m_pOwner->m_skin.GetArmBone1()->GetPos());
	//float minDisSqr = std::numeric_limits<float>::max();
	//m_nearestTouchPoint = NULL;
	//for (auto&a : m_pTouchPoints) {
	//	float disSqr = (a->GetPos() - point).LengthSquared();
	//	if (minDisSqr > disSqr) {
	//		minDisSqr = disSqr;
	//		m_nearestTouchPoint = a;
	//	}
	//}
//}





void PlayerSensor::updateRayCast() {
	//work with normal Ground angle
	if (m_newGroundNormal) {
		//calculate pelvis angle -> costly
		m_normalAngle = glm::degrees(b2Atan2(m_groundNormal.y, m_groundNormal.x));
		if (m_normalAngle < 0)m_normalAngle += 360.0f;
		else if (m_normalAngle >= 360.0f)m_normalAngle -= 360.0f;
		m_newGroundNormal = false;
	}
	//ray cast
	auto&pos = m_pOwner->GetBody()->GetPosition();
	m_minRayCastFraction = 1.0f;//initial
	Locator::GetPhysicsFactory()->GetB2World()->RayCast(this, pos+m_rayCastStartPoint, pos+m_rayCastEndPoint);
}

float32 PlayerSensor::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction)
{
	if (fixture->GetBody() == m_pOwner->GetBody())
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





void PlayerSensor::processContacts()
{
	//update the onGround state
	for (int i = 0; i < TOTAL_TOUCH_EDGE; i++) m_touchEdges[i] = false;
	float half_height = m_pOwner->GetBody()->GetFixtureList()->GetNext()->GetShape()->m_radius;

	auto center = m_pOwner->GetBody()->GetPosition() + b2Vec2(0.0f, m_pOwner->GetBody()->GetFixtureList()[0].GetShape()->m_radius);
	auto center2 = m_pOwner->GetBody()->GetPosition() - b2Vec2(0.0f, half_height);
	Locator::GetPrimitiveRenderer()->DrawCircle(Utils::b2ToGlm(M2P*center2), 10, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

	for (b2ContactEdge*contactEdge = m_pOwner->GetBody()->GetContactList(); contactEdge != nullptr; contactEdge = contactEdge->next) {

		b2Contact*contact = contactEdge->contact;
		if (contact->IsTouching()) {

			auto otherBody = contact->GetFixtureA()->GetBody();
			if (otherBody == m_pOwner->GetBody()) otherBody = contact->GetFixtureB()->GetBody();
			auto other = (BodyBase*)otherBody->GetUserData();

			if (other->GetSpecifier() == BOT_ROCK) {
				m_perceptClosestRock.m_pBody = other->GetBody();
				m_perceptClosestRock.m_type = PlayerPerceptType::PPT_CLOSEST_ROCK;
				m_perceptClosestRock.m_sensingMechanism = SensingMechanism::SM_TOUCHING;
			}


			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);

			//check if the point is below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_pOwner->GetBody()->GetPosition().y - half_height * 3.0f / 2.0f) {

					Locator::GetPrimitiveRenderer()->DrawCircle(
						M2P*Utils::b2ToGlm(manifold.points[i]), 5.0f,
						glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					m_touchEdges[BOTTOM] = true;
				}
				else if (manifold.points[i].y < m_pOwner->GetBody()->GetPosition().y - half_height * 5.0f / 4.0f) {
					if (manifold.points[i].x < m_pOwner->GetBody()->GetPosition().x) m_touchEdges[BOTTOM_LEFT] = true;
					else m_touchEdges[BOTTOM_RIGHT] = true;
				}
				else if (manifold.points[i].y < m_pOwner->GetBody()->GetPosition().y + half_height * 5.0f / 4.0f) {
					if (manifold.points[i].x < m_pOwner->GetBody()->GetPosition().x) {
						m_touchEdges[LEFT] = true;
						Locator::GetPrimitiveRenderer()->DrawCircle(
							M2P*Utils::b2ToGlm(manifold.points[i]), 5.0f,
							glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
					}
					else {
						m_touchEdges[RIGHT] = true;
						Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(manifold.points[i]), 5.0f,
							glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
					}
				}
				else if (manifold.points[i].y < m_pOwner->GetBody()->GetPosition().y + half_height * 3.0f / 2.0f) {
					if (manifold.points[i].x < m_pOwner->GetBody()->GetPosition().x) m_touchEdges[TOP_LEFT] = true;
					else m_touchEdges[TOP_RIGHT] = true;
				}
				else {
					m_touchEdges[TOP] = true;
					continue;
				}
				break;
			}
		}
	}
}

