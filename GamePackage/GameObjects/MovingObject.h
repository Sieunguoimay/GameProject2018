#pragma once
#include"BaseObject.h"
class SteeringBehaviors;
class MovingObject :public PhysicsObject {


	
	friend class SteeringBehaviors;
	//steering behavior use this .. not public outside. :))
	//movingObject...
	//what behaviors your are on-> Moving Objecr
	int32 m_flags;

	b2Vec2 m_target;
	//who are you pursuiting -> movingObject
	MovingObject*m_pEvader;
	//who is chasing you -> moving object for Evade() and Hide() also
	MovingObject*m_pPursuer;

	//wander -> moving object
	b2Vec2 m_wanderTarget;

	//Interpose -> movingObject
	MovingObject*m_pAgentA;
	MovingObject*m_pAgentB;

	//Follow path ->movingObject
	std::vector<b2Vec2>*m_pPath;
	int m_currentWaypoint;

	//offset pursuit ->movingObject
	MovingObject*m_pLeader;
	b2Vec2 m_offset;

	//flocking - temp
	std::vector<MovingObject*>m_neighbors;





	SteeringBehaviors*m_pSteering;
	b2Vec2 m_side;
	b2Vec2 m_heading;
	float m_headingAngle;
	float m_currentAngle;
	float m_maxSpeed;
	float m_maxForce;

	//utilities
	void CalculateAngle(float angle);
	float AngleArrive(float desiredAngle);
public:

	MovingObject(Sprite*sprite, float x,float y,float w, float h, int shape,SteeringBehaviors*steeringBehaviors);
	virtual ~MovingObject();
	virtual void ProcessCollision(UserData*other);

	virtual void Update(float deltaTime);
	virtual void Draw();


	//to serve for steering behaviors
	void ApplyForce(b2Vec2 force, b2Vec2 position);
	SteeringBehaviors*GetSteeringBehaviors();
	const float&GetMaxForce()const { return this->m_maxForce; }
	const float&GetMaxSpeed()const { return this->m_maxSpeed; }
	//overiding
	b2Vec2 GetPos();
	b2Vec2 GetVel();
	
	void SetHeading(b2Vec2 heading);
	const b2Vec2&GetHeading() const { return this->m_heading; }
	const b2Vec2&GetSide() const { return this->m_side; }

};
