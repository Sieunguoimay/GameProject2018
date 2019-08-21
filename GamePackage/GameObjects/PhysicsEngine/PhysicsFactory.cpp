#include "PhysicsFactory.h"
PhysicsFactory::PhysicsFactory(){}
PhysicsFactory::~PhysicsFactory(){}

void PhysicsFactory::Init()
{
	m_world = new b2World(b2Vec2(0.0f, -10.0f));
	m_contactListener = new ContactListener();
	m_world->SetContactListener(m_contactListener);
	createFixtureList();
}

void PhysicsFactory::CleanUp()
{
	LIST_FOR(it, m_actions) delete it->data;

	delete m_contactListener;
	for (auto&a : m_fixtureList) delete (FixtureUserData*)a.userData;
	delete m_world;
	
	m_world = NULL;
}

void PhysicsFactory::Update(float deltaTime)
{
	m_world->ClearForces();
	m_world->Step(deltaTime, 6, 2);
	m_world->DrawDebugData();

	LIST_FOR(it,m_actions){
		it->data->Run(deltaTime);
		if (it->data->Timeout()&&it->data->IsAutorelease()) {
			delete it->data;
			it = m_actions.erase(it);
		}
	}
}

void PhysicsFactory::SetRenderer(b2Draw * debugDraw)
{
	m_world->SetDebugDraw(debugDraw);
}



b2Body * PhysicsFactory::CreateBody(b2Shape * shape, const b2BodyType & type, const MaterialType & material, const b2Vec2 & pos)
{
	b2BodyDef bd;
	bd.type = type;
	bd.position.Set(pos.x,pos.y);//Example: if x is 100 then position.x is 1.0f
	b2Body*body = m_world->CreateBody(&bd);

	m_fixtureList[material].shape = shape;
	body->CreateFixture(&m_fixtureList[material]);

	return body;
}


void PhysicsFactory::createFixtureList() {

	//the order of creating that matters

	//super light and bounce, less friction
	m_fixtureList.push_back(b2FixtureDef()); 
	{
		b2FixtureDef& fd = m_fixtureList.back();
		fd.density = 1.0f;
		fd.friction = 0.2f;
		fd.restitution = 0.75f;
		fd.userData = new FixtureUserData(PLASTIC);
	}

	//light, no bounce and some friction
	m_fixtureList.push_back(b2FixtureDef());
	{
		b2FixtureDef& fd = m_fixtureList.back();
		fd.density = 10.0f;
		fd.friction = 0.5f;
		fd.restitution = 0.15f;
		fd.userData = new FixtureUserData(MEAT);

	}

	//heavy, some bounce, some friction
	m_fixtureList.push_back(b2FixtureDef());
	{
		b2FixtureDef& fd = m_fixtureList.back();
		fd.density = 20.0f;
		fd.friction = 0.5f;
		fd.restitution = 0.5f;
		fd.userData = new FixtureUserData(WOOD);

	}

	//heavy, no bounce, much friction
	m_fixtureList.push_back(b2FixtureDef());
	{
		b2FixtureDef& fd = m_fixtureList.back();
		fd.density = 30.0f;
		fd.friction = 0.75f;
		fd.restitution = 0.1f;
		fd.userData = new FixtureUserData(SOIL);

	}

	//super heavy, bounce and some friction
	m_fixtureList.push_back(b2FixtureDef());
	{
		b2FixtureDef& fd = m_fixtureList.back();
		fd.density = 50.0f;
		fd.friction = 0.3f;
		fd.restitution = 0.5f;
		fd.userData = new FixtureUserData(STEEL);

	}
	//SENSOR, NO MASS, just sensor
	m_fixtureList.push_back(b2FixtureDef());
	{
		b2FixtureDef& fd = m_fixtureList.back();
		fd.density = 0.0f;
		fd.isSensor = true;
		fd.userData = new FixtureUserData(SENSOR);

	}

}
b2FixtureDef & PhysicsFactory::GetFixture(MaterialType type, b2Shape * shape)
{
	if(shape!=NULL)
		m_fixtureList[type].shape = shape;
	return m_fixtureList[type];
	// TODO: insert return statement here
}
void PhysicsFactory::Distance(b2DistanceOutput * output, b2Shape * shapeA, const b2Transform & transformA, b2Shape * shapeB, const b2Transform & transformB)
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