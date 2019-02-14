#pragma once
#include"Box2D\Box2D.h"
class ContactListener :public b2ContactListener {
public:
	~ContactListener() override {}
	/// Called when two fixtures begin to touch.
	virtual void BeginContact(b2Contact* contact) override;
	/// Called when two fixtures cease to touch.
	virtual void EndContact(b2Contact* contact) override;
	/// Called when two fixtures about to touch.
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)override;
	/// Called when two fixtures no more touch.
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)override;

};