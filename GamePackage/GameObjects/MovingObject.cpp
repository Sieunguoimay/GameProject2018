#include"MovingObject.h"
#include"SteeringBehaviors.h"
#include"../Physics/PhysicsManager.h"
#include"../misc/GameAccessor.h"

MovingObject::MovingObject(Sprite*sprite, float x, float y,float w, float h, int shape, SteeringBehaviors*steeringBehaviors)
	:PhysicsObject(sprite,x,y, w, h),
	m_pSteering(steeringBehaviors),
	m_flags(none),
	m_pEvader(NULL),
	m_pPursuer(NULL),
	m_pAgentA(NULL),
	m_pAgentB(NULL),
	m_pPath(NULL),
	m_currentWaypoint(0)

{
	if (shape == BOX_SHAPE)
		m_physicsBody = s_physicsManager.CreateBoxBody(x,y, w, w, b2_dynamicBody);
	else if (shape == CIRCLE_SHAPE)
		m_physicsBody = s_physicsManager.CreateCircleBody(x,y, w, b2_dynamicBody);
	m_physicsBody->SetUserData(DEFAULT_OBJECT_TYPE, this);
	this->m_heading.Set(0, 1.0f);
	this->m_side.Set(-1.0f, 0.0f);
	this->m_headingAngle = b2_pi;
	this->m_currentAngle = b2_pi;
	this->m_maxForce = 20.0f;
	this->m_maxSpeed = 500.0f;

	if(m_pSteering)
		m_pSteering->SetOwner(this);//temporary for some setting outside that require
}
MovingObject::~MovingObject()
{
	m_pSteering = NULL;
	m_pEvader = NULL;
	m_pPursuer = NULL;
	m_pAgentA = NULL;
	m_pAgentB = NULL;
	m_pPath = NULL;

}
void MovingObject::ProcessCollision(UserData*other) {
}

void MovingObject::Update(float deltaTime) {
	if (m_pSteering != NULL) {
		m_pSteering->SetOwner(this);
		ApplyForce(m_pSteering->Calculate(deltaTime), m_physicsBody->GetBody()->GetWorldCenter());
	}
	PhysicsObject::Update(deltaTime);

	AngleArrive(m_headingAngle);
	CalculateAngle(m_currentAngle - b2_pi);
}
void MovingObject::Draw() {
	PhysicsObject::Draw();
}

void MovingObject::ApplyForce(b2Vec2 force, b2Vec2 position)
{
	m_physicsBody->GetBody()->ApplyForce(force, position, true);//how strong is the force??
}

SteeringBehaviors * MovingObject::GetSteeringBehaviors()
{
	if (m_pSteering != NULL) {
		this->m_pSteering->SetOwner(this);
		return this->m_pSteering;
	}
	return NULL;
}

b2Vec2 MovingObject::GetPos()
{
	return m_physicsBody->GetPos();//pixels
}

b2Vec2 MovingObject::GetVel()
{
	return m_physicsBody->GetVel();
}

void MovingObject::SetHeading(b2Vec2 heading)
{
	if (heading.LengthSquared() > 5.0f) {
		heading.Normalize();
		m_heading = heading;
		m_headingAngle = b2Atan2(heading.x, -heading.y);
		m_side.Set(-m_heading.y, m_heading.x);
		//SDL_Log("%f", m_headingAngle);
	}
}

void MovingObject::CalculateAngle(float angle)
{
	
	float fps = s_gameAccessor.GetFPS();
	float nextAngle = GetAngle()+m_physicsBody->GetBody()->GetAngularVelocity()/ fps;
	float totalRotation = angle - nextAngle;
	while (totalRotation < glm::radians(-180.0f)) totalRotation += glm::radians(360.0f);
	while (totalRotation > glm::radians(180.0f)) totalRotation -= glm::radians(360.0f);
	float desiredAngularVel = totalRotation * fps;
	float torque = m_physicsBody->GetBody()->GetInertia()*desiredAngularVel/(1.0f / fps);
	m_physicsBody->GetBody()->ApplyTorque(torque, true);
	//m_physicsBody->GetBody()->SetTransform(m_physicsBody->GetBody()->GetPosition(),desiredAngle);
}

float MovingObject::AngleArrive(float targetAngle) {
	float angle = m_currentAngle;

	while (angle < glm::radians(-180.0f)) angle += glm::radians(360.0f);
	while (angle > glm::radians(180.0f)) angle -= glm::radians(360.0f);

	float diff = targetAngle - angle;

	if (b2Abs(diff) >= b2_pi)
		diff = _sign(angle)*(b2_pi*2.0f + _sign(targetAngle)*(angle - targetAngle));

	m_currentAngle += diff*0.05f;
	/*
	while (m_currentAngle < glm::radians(-180.0f)) 
		m_currentAngle += glm::radians(360.0f);
	while (m_currentAngle > glm::radians(180.0f)) 
		m_currentAngle -= glm::radians(360.0f);
		*/
	return 0.0f;
}
