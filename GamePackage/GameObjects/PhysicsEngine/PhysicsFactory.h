#pragma once
#include<vector>
#include"Box2D\Box2D.h"
#include"ContactListener.h"
#include"PrimitiveActions.h"
#include"../../misc/list.h"

enum MaterialType {
	PLASTIC,
	MEAT,
	WOOD,
	SOIL,
	STEEL,
	SENSOR
};
struct FixtureUserData {
	FixtureUserData(MaterialType material)
		:material(material){}

	MaterialType material;
	//other information goes here..
};



class PhysicsFactory {
	b2World*m_world;
	ContactListener*m_contactListener;

	std::vector<b2FixtureDef>m_fixtureList;
	void createFixtureList();

	List<pe::BaseAction*> m_actions;

public:
	PhysicsFactory();
	~PhysicsFactory();

	void Init();
	void CleanUp();
	void Update(float deltaTime);
	
	int RunAction(pe::BaseAction*action) { m_actions.push_back(action); return m_actions.size()-1; }

	b2Body*CreateBody(b2Shape*shape, const b2BodyType& type, const MaterialType& material, const b2Vec2& pos);

	//getters and setters
	void SetRenderer(b2Draw*debugDraw);
	b2World*GetB2World() { return m_world; }
	b2FixtureDef&GetFixture(MaterialType type,b2Shape*shape = NULL);
	static void Distance(b2DistanceOutput*output,
		b2Shape*shapeA, const b2Transform&transformA,
		b2Shape*shapeB, const b2Transform&transformB);
};
