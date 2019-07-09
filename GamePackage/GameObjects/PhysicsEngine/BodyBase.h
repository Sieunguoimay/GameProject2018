#pragma once
#include"Box2D\Box2D.h"
#include"glm\glm.hpp"

//convension - quy uoc
//glm::vec2 ---> pixel unit
//b2Vec2 ------> meter unit
enum BodyObjectType{
	UNDEFINED,
	BOT_GROUND,
	BOT_PLAYER,
	BOT_TOUCH_POINT,
	BOT_ROCK 
};

struct ContactInfo {
	//continue the thinking of the solution for getting the contact point
	//in the future when you want it.
	//b2Vec2 points[b2_maxManifoldPoints];
	class BodyBase*other = NULL;
	bool hasContact = false;
};

class BodyBase {
protected:
	BodyObjectType m_bodyId;
	b2Body*m_body;

	ContactInfo m_contactInfo;
public:
	int _id;

	BodyBase():m_bodyId(UNDEFINED), m_body(NULL){}
	//virtual void Update(float deltaTime) {}				
	virtual void HandleBeginContact(b2Contact*contact, b2Fixture*fixture) {}
	virtual void HandleEndContact(b2Contact*contact, b2Fixture*fixture) {}	

	virtual void beginContact(BodyBase*other);
	void endContact();

	inline const BodyObjectType GetSpecifier() const { return m_bodyId; }
	inline virtual b2Body*GetBody() { return m_body; };			//be careful babe
};


inline void BodyBase::beginContact(BodyBase*other) {

	m_contactInfo.hasContact = true; 
	m_contactInfo.other = other;

}
inline void BodyBase::endContact() { m_contactInfo.hasContact = false; }