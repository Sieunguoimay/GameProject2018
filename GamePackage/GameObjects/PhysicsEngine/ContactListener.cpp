#include "ContactListener.h"
#include"BodyBase.h"





void ContactListener::BeginContact(b2Contact * contact)
{
	((BodyBase*)contact->GetFixtureA()->GetBody()->GetUserData())->HandleBeginContact(contact,contact->GetFixtureB());
	((BodyBase*)contact->GetFixtureB()->GetBody()->GetUserData())->HandleBeginContact(contact, contact->GetFixtureA());
}

void ContactListener::EndContact(b2Contact * contact)
{
	((BodyBase*)contact->GetFixtureA()->GetBody()->GetUserData())->HandleEndContact(contact, contact->GetFixtureB());
	((BodyBase*)contact->GetFixtureB()->GetBody()->GetUserData())->HandleEndContact(contact, contact->GetFixtureA());
}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
}
