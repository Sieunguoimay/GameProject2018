#pragma once
#include"../../PhysicsEngine/BodyBase.h"
#include"Leg.h"
#include"../TouchPoint.h"
class TerrestrialBody:public BodyBase, public b2RayCastCallback, public b2QueryCallback {

	b2Body*m_pBody;
	//debug purpose
	//b2WorldManifold m_manifold;
	//b2Fixture*m_groundFixture;

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

	//Radar
	bool ReportFixture(b2Fixture* fixture)override;
	TouchPoint*m_pTouchPoint = NULL;

public:
	TerrestrialBody();
	~TerrestrialBody();

	//how to install me
	void SetupBody(glm::vec2, glm::vec2, b2Body*&body);
	void Update(float deltaTime) override;
	

	//provide information about me
	inline b2Body*GetBody()override { return m_pBody; }
	inline const b2Vec2& GetGroundNormal()const { return m_groundNormal; }
	inline const float& GetGroundNormalAngle()const { return m_normalAngle; }
	inline bool IsOnGround() const { return m_onGround; }


	//what i can do
	void Stop();
	void Run(bool left_right);//true is right, false is left
	void Jump();
	void JumpAndRun(bool left_right);
	
	inline TouchPoint*GetTouchPoint() { return m_pTouchPoint; }
};
