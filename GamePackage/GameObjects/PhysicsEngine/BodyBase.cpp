#include "BodyBase.h"
#include"../../misc/Locator.h"
BodyBase::~BodyBase()
{
	Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
}
