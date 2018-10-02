#pragma once
#include"PhysicsBody.h"
#include"Box2D\Box2D.h"
#include<vector>
class PhysicsManager :public b2ContactListener{
	PhysicsManager(){}
	PhysicsManager(const PhysicsManager&);
	PhysicsManager&operator = (const PhysicsManager&);
	static PhysicsManager*s_instance;
	float m_ratio;
	b2World*m_world;
public:
	static PhysicsManager&Instance();
	~PhysicsManager();
	bool Init(b2Vec2 gravity,float ratio);
	void CleanUp();
	void Update(float deltaTime);

	PhysicsBody*CreateChainBody(std::vector<b2Vec2> chain);
	PhysicsBody*CreateBoxBody(float x, float y, float w, float h,b2BodyType type);
	PhysicsBody*CreateCircleBody(float x, float y, float radius, b2BodyType type);
	b2MouseJoint*CreateMouseJoint(float x, float y, b2Body*body);

	//Getters and setters
	b2World*GetWorld() { return this->m_world; }

	//override
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);


};
#define s_physicsManager PhysicsManager::Instance()