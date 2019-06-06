#pragma once
#include"Box2D\Box2D.h"
#include"../../../misc/StateMachine/StateMachine.h"
class Leg 
	:public b2QueryCallback
	,public b2RayCastCallback {

	b2Body*m_pBody;

	bool ReportFixture(b2Fixture*fixture)override;
	float32 ReportFixture(b2Fixture*fixture, const b2Vec2&point, const b2Vec2&normal, float32 fraction)override;

	float m_minRayCastFraction;
	b2Vec2 m_groundNormal;

	float m_legLength;
	float m_stepLength;
	b2Vec2 m_footPos;
	b2Vec2 m_footPos1;
	b2Vec2 m_footPos2;
	bool m_flag = false;
	bool m_flagSwitchingFoot = false;


	float m_normalAngle;
	bool m_newGroundNormal = false;//dirty flag
public:
	Leg();
	~Leg();
	void Init(b2Body*pBody);
	void Update(float deltaTime);

	inline const b2Vec2& GetGroundNormal()const { return m_groundNormal; }
	inline const float& GetGroundNormalAngle()const { return m_normalAngle; }
};
