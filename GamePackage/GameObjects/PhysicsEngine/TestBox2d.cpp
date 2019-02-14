#include "TestBox2d.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"

TestBox2D::TestBox2D(b2World * pWorld)
	:m_pWorld(pWorld)
	,dj(NULL)
{
	b2PolygonShape ps;
	ps.SetAsBox(0.5f, 0.5f);
	body = Locator::GetPhysicsFactory()->CreateBody(
		&ps, b2_dynamicBody, MaterialType::PLASTIC, b2Vec2(2.0f, 2.0f));

	SDL_Log("Mass: %f", body->GetMass());
}

void TestBox2D::Update(float deltaTime)
{

}
void TestBox2D::Draw()
{
	{
		//distance
		//b2Body*playerBody = m_pWorld->GetBodyList()->GetNext()->GetNext();
		//b2DistanceOutput output;
		//Distance(&output, body->GetFixtureList()->GetShape(), body->GetTransform(),
		//	playerBody->GetFixtureList()->GetShape(),
		//	playerBody->GetTransform());

		//Locator::GetPrimitiveRenderer()->DrawLine(
		//	glm::vec2(output.pointA.x*M2P, output.pointA.y*M2P),
		//	glm::vec2(output.pointB.x*M2P, output.pointB.y*M2P)
		//	);


		//b2Vec2 diffPos = output.pointA- output.pointB;
		//b2Vec2 diffVel = body->GetLinearVelocity() - playerBody->GetLinearVelocity();

		//if (b2Dot(diffPos, diffVel) < 0) {
		//	if (output.distance*M2P <= 80) {

		//		if (dj == NULL) {

		//			b2DistanceJointDef djd;
		//			djd.Initialize(body, playerBody, output.pointA,output.pointB);
		//			djd.collideConnected = true;
		//			djd.dampingRatio = 0.0f;
		//			djd.frequencyHz = 2.0f;
		//			dj = (b2DistanceJoint*)m_pWorld->CreateJoint(&djd);
		//		}
		//	}

		//}else
		//if (output.distance*M2P > 80) {
		//		if (dj != NULL) {
		//			m_pWorld->DestroyJoint(dj);
		//			dj = NULL;

		//		}
		//}

		//if(dj!=NULL)
		//	Locator::GetPrimitiveRenderer()->DrawLine(glm::vec2(dj->GetAnchorA().x, dj->GetAnchorA().y)*M2P,
		//		glm::vec2(dj->GetAnchorB().x, dj->GetAnchorB().y)*M2P);


		//query for aabb 
		//b2AABB aabb;
		//aabb.lowerBound.Set(playerBody->GetPosition().x-150/M2P, playerBody->GetPosition().y-150 / M2P);
		//aabb.upperBound.Set(playerBody->GetPosition().x+150 / M2P, playerBody->GetPosition().y+150 / M2P);

		//m_pWorld->QueryAABB(this, aabb);

		//Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
		//	aabb.lowerBound.x*M2P, aabb.lowerBound.y*M2P,
		//	(aabb.upperBound.x - aabb.lowerBound.x)*M2P,
		//	(aabb.upperBound.y - aabb.lowerBound.y)*M2P), 0);


	}



	//ray cast 
	const glm::vec2&pos = Locator::GetInput()->GetMousePosInWorld();
	b2Vec2 p1 = b2Vec2(0.0,0.0);
	b2Vec2 p2 = b2Vec2(pos.x/ M2P, pos.y/ M2P);
	m_pWorld->RayCast(this,p1,p2);

}

bool TestBox2D::ReportFixture(b2Fixture * fixture)
{
	b2AABB aabb = fixture->GetAABB(0);
	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
		aabb.lowerBound.x*M2P, aabb.lowerBound.y*M2P,
		(aabb.upperBound.x- aabb.lowerBound.x)*M2P, 
		(aabb.upperBound.y- aabb.lowerBound.y)*M2P),0);

	return true;
}

float32 TestBox2D::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction)
{
	b2Vec2 hitPoint = point ;

	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(hitPoint.x*M2P, hitPoint.y*M2P,
		10, 10), 0, glm::vec2(0, 0));

	Locator::GetPrimitiveRenderer()->DrawLine(glm::vec2(0, 0),
		glm::vec2(hitPoint.x*M2P, hitPoint.y*M2P));

	return fraction;
}

void TestBox2D::Distance(b2DistanceOutput*output, 
	b2Shape*shapeA, const b2Transform&transformA,
	b2Shape*shapeB, const b2Transform&transformB)
{
	b2DistanceInput input;
	b2SimplexCache simplexCache;
	simplexCache.count = 0;
	input.proxyA.Set(shapeA, 0);
	input.proxyB.Set(shapeB, 0);
	input.transformA = transformA;
	input.transformB = transformB;
	b2Distance(output, &simplexCache, &input);
}
