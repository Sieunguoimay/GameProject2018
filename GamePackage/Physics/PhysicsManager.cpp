#include"PhysicsManager.h"
#include"../GameObjects/BaseObject.h"
#include"../misc/GameAccessor.h"
PhysicsManager*PhysicsManager::s_instance = NULL;
PhysicsManager&PhysicsManager::Instance() {
	if (s_instance == NULL)
		s_instance = new PhysicsManager();
	return *s_instance;
}
PhysicsManager::~PhysicsManager() {

}

//put these things in public place 
bool PhysicsManager::Init(b2Vec2 gravity,float ratio) {
	this->m_ratio = ratio;
	this->m_world = new b2World(gravity);
	this->m_world->SetContactListener(this);
	b2EdgeShape es;
	b2ChainShape cs;
	//es.Set(,);
	return true;
}
void PhysicsManager::CleanUp() {
	delete m_world;
	delete s_instance;
}
void PhysicsManager::Update(float deltaTime) {
	m_world->Step(deltaTime, 8, 3);
}

void PhysicsManager::PreSolve(b2Contact * contact, const b2Manifold * oldManifold) {

	void*userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void*userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	if (userDataA) {
		//PhysicsObject*objectA = (PhysicsObject*)(((UserData*)userDataA)->pointer);
		//objectA->ProcessCollision((UserData*)userDataB);
	}
	if (userDataB) {
		//PhysicsObject*objectB = (PhysicsObject*)(((UserData*)userDataB)->pointer);
		//objectB->ProcessCollision((UserData*)userDataA);
	}
}

void PhysicsManager::BeginContact(b2Contact * contact)
{
}

void PhysicsManager::EndContact(b2Contact * contact)
{
}



PhysicsBody * PhysicsManager::CreateChainBody(std::vector<b2Vec2> chain)
{
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(0.0f, 0.0f);
	b2Body*newBody = m_world->CreateBody(&bd);

	for (auto&a : chain) a *= 1.0f/m_ratio;

	b2ChainShape chs;
	chs.CreateChain(&(chain[0]), chain.size());

	b2FixtureDef fd;
	fd.shape = &chs;
	newBody->CreateFixture(&fd);

	PhysicsBody*newPhysicsBody = new PhysicsBody(m_world, newBody);
	newPhysicsBody->SetRatio(m_ratio);
	return newPhysicsBody;
}

//use this to create Body Object
PhysicsBody*PhysicsManager::CreateBoxBody(float x, float y, float w, float h, b2BodyType type) {
	b2BodyDef bd;
	bd.type = type;
	bd.position.x = x / m_ratio;//Example: if x is 100 then position.x is 1.0f
	bd.position.y = y / m_ratio;//where m_ratio = 100.0f
	b2Body*newBody = m_world->CreateBody(&bd);
	
	b2PolygonShape ps;
	ps.SetAsBox(0.5f*w / m_ratio, 0.5f*h / m_ratio);

	b2FixtureDef fd;
	fd.shape = &ps;
	fd.density = 0.2f;
	fd.restitution = 0.5f;
	fd.friction = 0.3f;
	newBody->CreateFixture(&fd);

	PhysicsBody*newPhysicsBody = new PhysicsBody(m_world,newBody);
	newPhysicsBody->SetRatio(m_ratio);
	return newPhysicsBody;
}
PhysicsBody*PhysicsManager::CreateCircleBody(float x, float y, float radius, b2BodyType type) {
	b2BodyDef bd;
	bd.type = type;
	bd.position.x = x / m_ratio;//Example: if x is 100 then position.x is 1.0f
	bd.position.y = y / m_ratio;//where m_ratio = 100.0f
	b2Body*newBody = m_world->CreateBody(&bd);
	
	b2CircleShape cs;
	cs.m_radius = 0.5f*radius/m_ratio;

	b2FixtureDef fd;
	fd.shape = &cs;
	fd.density = 1.0f;
	fd.restitution = 0.5f;
	fd.friction = 0.3f;
	newBody->CreateFixture(&fd);

	PhysicsBody*newPhysicsBody = new PhysicsBody(m_world, newBody);
	newPhysicsBody->SetRatio(m_ratio);
	return newPhysicsBody;
}

b2MouseJoint * PhysicsManager::CreateMouseJoint(float x, float y, b2Body * body)
{
	b2MouseJointDef md;
	return nullptr;
}

