#pragma once
#include<Box2D\Box2D.h>
class BodyDragAndDrop {
	b2MouseJoint*m_joint = NULL;
	b2Body*m_groundBody = NULL;
	b2World*m_pWorld;
	bool m_isFixedRotation;

public:
	BodyDragAndDrop(b2World*world) {
		b2BodyDef bodyDef;
		m_groundBody = world->CreateBody(&bodyDef);
		m_pWorld = world;
		m_isFixedRotation = false;
	}
	virtual ~BodyDragAndDrop() {
		//if (m_joint != NULL) Drop();
		if (m_groundBody != NULL)m_pWorld->DestroyBody(m_groundBody);
		m_pWorld = NULL;
	}
	void DragStart(b2Body*body, const b2Vec2& point) {
		b2MouseJointDef md;
		md.bodyA = m_groundBody;
		md.bodyB = body;
		md.target = point;
		md.maxForce = 1000.0f * body->GetMass();
		m_joint = (b2MouseJoint*)m_pWorld->CreateJoint(&md);
		body->SetAwake(true);
		if (!body->IsFixedRotation())
			body->SetFixedRotation(true);
		else
			m_isFixedRotation = true;
	}
	void SetTarget(const b2Vec2&point) {
		m_joint->SetTarget(point);
	}
	void Drop() {
		

		if (!m_isFixedRotation)
			m_joint->GetBodyB()->SetFixedRotation(false);
		m_pWorld->DestroyJoint(m_joint);
		m_joint = NULL;
	}
};
