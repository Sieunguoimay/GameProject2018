#pragma once
#include"../../PhysicsEngine/BodyBase.h"
#include"../TouchPoint.h"

#define RUN_SPEED 2.0f
#define WALK_SPEED 1.3f

class TerrestrialBody:public BodyBase, public b2RayCastCallback, public b2QueryCallback,public Entity {

	bool m_onGround;
	float m_jumpSpeed;
	float m_runSpeed;

	//ground normal
	float m_normalAngle;
	bool m_newGroundNormal = false;//dirty flag

	//box2d support
	float m_minRayCastFraction;
	b2Vec2 m_groundNormal;

	//inner works
	float32 ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction);
	void processCollision();

	//Radar for hand control
	bool ReportFixture(b2Fixture* fixture)override;
	std::vector<TouchPoint*>m_pTouchPoints;
	TouchPoint*m_nearestTouchPoint;
	b2DistanceJoint*m_joint = NULL;

	b2Vec2 m_dir;
public:
	TerrestrialBody();
	~TerrestrialBody();

	//how to install me
	void SetupBody(glm::vec2, glm::vec2, b2Body*&body,void*parent);
	void Update(float deltaTime);


	//provide information about me
	inline b2Body*GetBody()override { return m_body; }
	inline const b2Vec2& GetGroundNormal()const { return m_groundNormal; }
	inline const float& GetGroundNormalAngle()const { return m_normalAngle; }
	inline bool IsOnGround() const { return m_onGround; }


	//what i can do
	void Stop();
	void Move(bool left_right, float speed = RUN_SPEED);//true is right, false is left
	void Jump();
	void JumpAndRun(bool left_right);
	void HoldTouchPoint(const b2Vec2&point);
	void LeaveTouchPoint();

	TouchPoint*GetNearestTouchPoint(const b2Vec2& point);
};
