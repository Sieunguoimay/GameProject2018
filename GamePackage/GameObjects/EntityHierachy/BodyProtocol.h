#pragma once
#include"Box2D/Box2D.h"

//answer the question: who are you . to the other object
//by prviding a common list of identification.
//and play the role of an interface with the contact listener

class BodyProtocol {
public:
	enum ObjectType {
		NULL_TYPE,
		PLAYER,
	};
	
	BodyProtocol(ObjectType type):m_objectType(type){}
	
	virtual void OnBeginContact(b2Contact*contact, b2Fixture*fixture) {}
	virtual void OnEndContact(b2Contact*contact, b2Fixture*fixture) {}

	inline const ObjectType& GetObjectType() { return m_objectType; }
protected:
	ObjectType m_objectType;
};
