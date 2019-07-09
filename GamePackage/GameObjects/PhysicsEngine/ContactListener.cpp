#include "ContactListener.h"
#include"../EntityHierachy/Entity.h"
//#include"../EntityHierachy/BodyProtocol.h"


//virtual function nevel gets called here................

ContactListener::ContactListener()
	:b2ContactListener()
{
}
ContactListener::~ContactListener() {

}

void ContactListener::BeginContact(b2Contact * contact)
{
	void*A = contact->GetFixtureA()->GetBody()->GetUserData();
	void*B = contact->GetFixtureB()->GetBody()->GetUserData();
	if (A&&B) {
		//SDL_Log("%d %d", ((BodyBase*)A)->GetSpecifier(), ((BodyBase*)B)->GetSpecifier());
		SDL_Log("%d %d", ((Entity*)A)->id, ((Entity*)B)->id);
		//SDL_Log("%d %d", ((BodyBase*)A)->_id, ((BodyBase*)B)->_id);
	}

}
// what i'm gonna do is:
// handle the touching of the player's body with the rock.
// after that if player is running toward the rock, and touch the rock then it will walk forward on the rock ahead direction
// and we will not create a new joint on that situation. we only create it if the rock is not on the direction of the player 

void ContactListener::EndContact(b2Contact * contact)
{
	//void*A = contact->GetFixtureA()->GetBody()->GetUserData();
	//void*B = contact->GetFixtureB()->GetBody()->GetUserData();
	//if (A&&B) {
	//	((BodyBase*)A)->endContact();
	//	((BodyBase*)B)->endContact();
	//}

}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
}
