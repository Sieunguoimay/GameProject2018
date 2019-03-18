#include "ContactListener.h"
//#include"BodyBase.h"
#include"../EntityHierachy/BodyProtocol.h"




void ContactListener::BeginContact(b2Contact * contact)
{
	//((BodyProtocol*)contact->GetFixtureA()->GetBody()->GetUserData())->OnBeginContact(contact,contact->GetFixtureB());
	//((BodyProtocol*)contact->GetFixtureB()->GetBody()->GetUserData())->OnBeginContact(contact, contact->GetFixtureA());
}

void ContactListener::EndContact(b2Contact * contact)
{
	//((BodyProtocol*)contact->GetFixtureA()->GetBody()->GetUserData())->OnEndContact(contact, contact->GetFixtureB());
	//((BodyProtocol*)contact->GetFixtureB()->GetBody()->GetUserData())->OnEndContact(contact, contact->GetFixtureA());
}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
}
