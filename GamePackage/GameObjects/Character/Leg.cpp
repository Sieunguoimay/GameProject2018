#include "Leg.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"
#include"../../misc/Math/Geometry.h"
Leg::Leg(){}
Leg::~Leg(){}


void Leg::Init(b2Body * pBody)
{
	m_pBody = pBody;
	m_legLength = 0.85f;

	m_footPos.Set(-1, -1);
}

void Leg::Update(float deltaTime)
{

	if (m_newGroundNormal) {
		//calculate pelvis angle -> costly
		m_normalAngle = glm::degrees(b2Atan2(m_groundNormal.y, m_groundNormal.x));
		if (m_normalAngle< 0)m_normalAngle += 360.0f;
		else if (m_normalAngle >= 360.0f)m_normalAngle -= 360.0f;
		m_newGroundNormal = false;
	}





	b2Vec2 vel = m_pBody->GetLinearVelocity();
	b2Vec2 pos = m_pBody->GetPosition();


	//walk(vel,deltaTime);


	//b2AABB aabb;
	//aabb.lowerBound = m_pBody->GetPosition() - b2Vec2(1.0f, 1.0f);
	//aabb.upperBound = m_pBody->GetPosition() + b2Vec2(1.0f, 1.0f);
	//Locator::GetPhysicsFactory()->GetB2World()->QueryAABB(this, aabb);

	//Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
	//	aabb.lowerBound.x*M2P, aabb.lowerBound.y*M2P,
	//	(aabb.upperBound.x - aabb.lowerBound.x)*M2P,
	//	(aabb.upperBound.y - aabb.lowerBound.y)*M2P), 0);


	//ray cast
	b2Vec2 p1 = pos;
	b2Vec2 p2 = pos + b2Vec2(0.0, -2.0f);
	m_minRayCastFraction = 1.0f;
	Locator::GetPhysicsFactory()->GetB2World()->RayCast(this, p1, p2);

	b2Vec2 hitPoint = (m_minRayCastFraction*(p2 - p1) + p1);
	Locator::GetPrimitiveRenderer()->DrawLine(M2P*glm::vec2(p1.x,p1.y),M2P*glm::vec2(hitPoint.x, hitPoint.y));
	//Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(M2P*hitPoint.x, M2P*hitPoint.y, 10, 10), 0, glm::vec2(0, 0));

	//ground line
	b2Vec2 s1 = hitPoint - b2Vec2(-m_groundNormal.y, m_groundNormal.x);
	b2Vec2 s2 = hitPoint + b2Vec2(-m_groundNormal.y, m_groundNormal.x);
	Locator::GetPrimitiveRenderer()->DrawLine(M2P*glm::vec2(s1.x,s1.y), 
		M2P*glm::vec2(s2.x, s2.y),
		glm::vec4(1.0,0.0,0.0,1.0));

	//leg circle
	//if (!m_flag) {
	//	m_footPos = hitPoint;
	//	m_footPos1 = m_footPos;
	//	m_footPos2 = m_footPos;
	//}
	//Geometry::CirlceIntersectOutput output;
	//Geometry::Intersection(pos, m_legLength, s1, s2, &output);
	//if (output.type >= 0) {

	//	if (!m_flag) m_flag = true;
	//	
	//	b2Vec2 dir(m_groundNormal.y, m_groundNormal.x);
	//	float touch_surface = b2Dot(output.p2 - output.p1, dir);
	//	if (touch_surface < 0) {
	//		b2Swap(output.p1, output.p2);
	//		touch_surface *= -1;
	//	}

	//	if (!m_footInTheAir.IsOnAir()) {
	//		float a = b2Dot(m_footPos - output.p1, dir);
	//		if (a < 0) {
	//			m_footInTheAir.OnAir(0.5f);
	//		}
	//		else if (a>touch_surface)
	//			m_footInTheAir.OnAir(0.5f);
	//	}
	//	else {
	//		m_footInTheAir.Update(deltaTime, output.p1, output.p2, m_groundNormal);
	//		m_footPos = m_footInTheAir.GetPos();
	//	}


	//	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(M2P*output.p1.x, M2P*output.p1.y, 10, 10), 0);
	//	if (output.type>0)
	//		Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(M2P*output.p2.x, M2P*output.p2.y, 10, 10), 0, glm::vec2(0.0f), glm::vec4(1.0, 0.0, 0.0, 1.0));

	//	//if (a<0 || a>touch_surface) {
	//	//	if (m_flagSwitchingFoot) {
	//	//		m_flagSwitchingFoot = false;
	//	//		m_footPos1 = m_footPos;
	//	//	}
	//	//	else {
	//	//		m_flagSwitchingFoot = true;
	//	//		m_footPos2 = m_footPos;
	//	//	}
	//	//}

	//	//if (!m_flagSwitchingFoot) {
	//	//	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(M2P*m_footPos1.x, M2P*m_footPos1.y, 10, 10), 0, glm::vec2(0.0f), glm::vec4(0.0, 1.0, 0.0, 1.0));
	//	//}
	//	//else {
	//	//	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(M2P*m_footPos2.x, M2P*m_footPos2.y, 10, 10), 0);
	//	//}
	//}
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*glm::vec2(pos.x, pos.y), m_legLength*M2P);


	//Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(M2P*m_footPos.x, M2P*m_footPos.y, 10, 10), 0);
	//Locator::GetPrimitiveRenderer()->DrawLine(M2P*glm::vec2(pos.x, pos.y),
	//	M2P*glm::vec2(m_footPos.x, m_footPos.y),
	//	glm::vec4(0.0, 1.0, 1.0, 1.0));


	//ground normal
	s1 = hitPoint;
	s2 = hitPoint+0.5f*m_groundNormal;
	Locator::GetPrimitiveRenderer()->DrawLine(M2P*glm::vec2(s1.x, s1.y), M2P*glm::vec2(s2.x, s2.y),
		glm::vec4(0.0, 0.0, 1.0, 1.0));

	//b2Vec2 points[] = { b2Vec2(-1,0),b2Vec2(-1,1),b2Vec2(1,1),b2Vec2(1,0) };

	//if (Locator::GetInput()->IsMousePressed()) {
	//	points[1].x = Locator::GetInput()->GetFirstPressedPosInWorld().x / M2P;
	//	points[1].y = Locator::GetInput()->GetFirstPressedPosInWorld().y / M2P;
	//}
	//for (float i = 0.0; i < 1.05; i += 0.1) {
	//	b2Vec2 p = Geometry::BezierCurve4(points[0], points[1], points[2], points[3], i);
	//	Locator::GetPrimitiveRenderer()->DrawCircle(M2P*glm::vec2(p.x, p.y), 5);
	//}
}


bool Leg::ReportFixture(b2Fixture * fixture)
{
	//for (b2Fixture*a = m_pBody->GetFixtureList(); a; a = a->GetNext()) 
	//	if (fixture == a)
	//		return true;
	//b2AABB aabb = fixture->GetAABB(0);
	//Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
	//	aabb.lowerBound.x*M2P, aabb.lowerBound.y*M2P,
	//	(aabb.upperBound.x - aabb.lowerBound.x)*M2P,
	//	(aabb.upperBound.y - aabb.lowerBound.y)*M2P), 0);
	return true;
}

float32 Leg::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction)
{
	for (b2Fixture*a = m_pBody->GetFixtureList(); a; a = a->GetNext())
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
