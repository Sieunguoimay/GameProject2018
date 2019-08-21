#pragma once
#include"Box2D\Box2D.h"
#include"BodyBase.h"
class ContactListener :public b2ContactListener {
public:
	/// Called when two fixtures begin to touch.
	void BeginContact(b2Contact* contact) override;
	/// Called when two fixtures cease to touch.
	void EndContact(b2Contact* contact) override;
	/// Called when two fixtures about to touch.
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)override;
	/// Called when two fixtures no more touch.
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)override;

};