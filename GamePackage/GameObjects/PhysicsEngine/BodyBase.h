#pragma once
#include"Box2D\Box2D.h"
#include"glm\glm.hpp"
#include"../../misc/list.h"
//convension - quy uoc
//glm::vec2 ---> pixel unit
//b2Vec2 ------> meter unit
enum BodyObjectType{
	UNDEFINED,
	BOT_GROUND,
	BOT_PLAYER,
	BOT_TOUCH_POINT,
	BOT_ROCK,

	BOT_TOTAL_NUMBER
};

enum ContactState {
	NO_CONTACT,
	CONTACT_BEGAN,
	CONTACT_HAPPENNING,
	CONTACT_ENDED
};

class BodyBase {
protected:
	
	BodyBase*m_bodyBaseThis; //look funny =)))
	
	b2Body*m_body;
	int m_bodyId;

public:

	BodyBase() :m_bodyId(UNDEFINED), m_body(NULL), m_bodyBaseThis(this) { }
	~BodyBase();
	virtual void HandleBeginContact(b2Contact*contact, b2Fixture*fixture){}
	virtual void HandleEndContact(b2Contact*contact, b2Fixture*fixture){}

	int GetSpecifier() const { return m_bodyId; }
	b2Body*GetBody() { return m_body; };			//be careful babe
};


