#pragma once
#include"Box2D\Box2D.h"
#include"glm\glm.hpp"

//convension - quy uoc
//glm::vec2 ---> pixel unit
//b2Vec2 ------> meter unit
enum BodySpecifier{
	UNDEFINED,
	GROUND,
	PLAYER
};
class BodyBase {
protected:
	glm::vec2 m_pos;//buffer
	BodySpecifier m_specifier;
public:
	BodyBase(glm::vec2 pos):m_pos(pos),m_specifier(UNDEFINED){}
	virtual void Update(float deltaTime) {}				//not compulsory
	virtual void HandleBeginContact(b2Contact*contact,b2Fixture*fixture) {}	//not compulsory
	virtual void HandleEndContact(b2Contact*contact, b2Fixture*fixture) {}	//not compulsory
	virtual b2Body*GetBody() { return NULL; };			//be careful babe

	inline const BodySpecifier& GetSpecifier() const { return m_specifier; }
	inline glm::vec2*GetPosPointer() { return &m_pos; }
};
