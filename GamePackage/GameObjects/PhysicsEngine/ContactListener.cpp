#include "ContactListener.h"
#include"../EntityHierachy/HavingBodyEntity.h"
//#include"../EntityHierachy/BodyProtocol.h"


//virtual function nevel gets called here................

void ContactListener::BeginContact(b2Contact * contact)
{
	void*A = contact->GetFixtureA()->GetBody()->GetUserData();
	void*B = contact->GetFixtureB()->GetBody()->GetUserData();
	if (A&&B) {
		((BodyBase*)A)->HandleBeginContact(contact, contact->GetFixtureB());
		((BodyBase*)B)->HandleBeginContact(contact, contact->GetFixtureA());
	}

}
// what i'm gonna do is:
// handle the touching of the player's body with the rock.
// after that if player is running toward the rock, and touch the rock then it will walk forward on the rock ahead direction
// and we will not create a new joint on that situation. we only create it if the rock is not on the direction of the player 

void ContactListener::EndContact(b2Contact * contact)
{
	void*A = contact->GetFixtureA()->GetBody()->GetUserData();
	void*B = contact->GetFixtureB()->GetBody()->GetUserData();
	if (A&&B) {
		((BodyBase*)A)->HandleEndContact(contact, contact->GetFixtureB());
		((BodyBase*)B)->HandleEndContact(contact, contact->GetFixtureA());
	}
}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
}
