#pragma once
#include"Box2D\Box2D.h"
#include"glm\glm.hpp"

//convension - quy uoc
//glm::vec2 ---> pixel unit
//b2Vec2 ------> meter unit
enum ObjectType{
	UNDEFINED,
	GROUND,
	OID_PLAYER,
	OID_TOUCH_POINT,
};

class BodyBase {
protected:
	ObjectType m_specifier;
public:

	BodyBase():m_specifier(UNDEFINED){}

	virtual void Update(float deltaTime) {}				
	virtual void HandleBeginContact(b2Contact*contact,b2Fixture*fixture) {}	
	virtual void HandleEndContact(b2Contact*contact, b2Fixture*fixture) {}	

	inline const ObjectType& GetSpecifier() const { return m_specifier; }
	inline virtual b2Body*GetBody() { return NULL; };			//be careful babe
};
