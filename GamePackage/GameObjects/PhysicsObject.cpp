#include"BaseObject.h"
#include"../Physics/PhysicsManager.h"

PhysicsObject::PhysicsObject(Sprite*sprite, float x, float y, int w, int h)
	:BaseObject(sprite,x,y,w,h)
{
	//pass in something that hardly changes
}
PhysicsObject::~PhysicsObject() {
	delete m_physicsBody;
}


void PhysicsObject::Update(float deltaTime)
{
	m_pos = m_physicsBody->GetPos();
	m_angle = m_physicsBody->GetAngle();
	m_animatedSprite->Update(deltaTime);
}

void PhysicsObject::Draw()
{
	m_animatedSprite->Draw(m_pos.x, m_pos.y, m_width, m_height, glm::degrees(m_angle));
}


//////////////////////////////////////////////////////////

StandStillObject::StandStillObject(Sprite * sprite, float x, float y, int w, int h, int shape)
	:PhysicsObject(sprite,x,y,w,h)
{
	if (shape == BOX_SHAPE)
		m_physicsBody = s_physicsManager.CreateBoxBody(x, y, w, h, b2_staticBody);
	else if(shape == CIRCLE_SHAPE)
		m_physicsBody = s_physicsManager.CreateCircleBody(x, y, w, b2_staticBody);
	m_physicsBody->SetUserData(DEFAULT_OBJECT_TYPE, this);
}
StandStillObject::~StandStillObject() {
}

void StandStillObject::ProcessCollision(UserData*other) {

	if (other->type == DEFAULT_OBJECT_TYPE) {
		//SDL_Log("Hey");
		//((PhysicsObject*)(other->pointer))->GetPhysicsBody()->GetBody()->ApplyForce(b2Vec2(0.0f,100.0f),b2Vec2(0.0,0.0),true);
		//((PhysicsObject*)(other->pointer))->GetAnimatedSprite()->SetAnimation("walk");
	}
}

void StandStillObject::Update(float deltaTime) {
	PhysicsObject::Update(deltaTime);
}
void StandStillObject::Draw() {
	PhysicsObject::Draw();
}

////////////////////////////////////////////////////////////////////////
