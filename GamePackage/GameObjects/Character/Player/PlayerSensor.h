#pragma once
#include"../TouchPoint.h"
#include"../../../misc/StateMachine/StateMachine.h"
#include"Percept.h"

enum SensingMechanism {
	SM_AABB_QUERY,
	SM_RAY_CASTING,
	SM_TOUCHING,
	SM_TOTAL_NUM
};
class PlayerSensor
	: public b2QueryCallback
	, public b2RayCastCallback{

	
	class Player*m_pOwner;

	Percept m_perceptClosestRock;

	//Radar for hand control AABB query
	b2AABB m_radarAABB;
	Node<b2Vec2>*m_iterator;
	List<b2Vec2> m_terrain;
	void updateAABBRadar();
	bool ReportFixture(b2Fixture* fixture)override;


	//Ray Cast Query
	float m_normalAngle;
	bool m_newGroundNormal = false;//dirty flag
	float m_minRayCastFraction;
	b2Vec2 m_groundNormal;

	b2Vec2 m_rayCastStartPoint;
	b2Vec2 m_rayCastEndPoint;
	void updateRayCast();
	float32 ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction);


	//Touch sensing
	enum TouchEdge {
		LEFT,
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		BOTTOM,
		BOTTOM_LEFT,
		TOTAL_TOUCH_EDGE
	};
	bool m_touchEdges[TOTAL_TOUCH_EDGE];
	void processContacts();
public:

	PlayerSensor(Player*pOwner);
	~PlayerSensor();

	void Update(float deltaTime);

	void SetRayCastStartPoint(const b2Vec2&p)	{ m_rayCastStartPoint = p; }
	void SetRayCastEndPoint(const b2Vec2&p)		{ m_rayCastEndPoint = p; }
	const b2Vec2& GetGroundNormal()const		{ return m_groundNormal; }
	const float& GetGroundNormalAngle()const	{ return m_normalAngle; }
	bool IsOnGround() const						{ return m_touchEdges[BOTTOM]; }
};