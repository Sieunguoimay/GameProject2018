#pragma once
#include"../PhysicsEngine/BodyBase.h"
#include"Leg.h"

class TerrestrialBody:public BodyBase {

	b2Body*m_body;
	Leg m_leg;
	//debug purpose
	//b2WorldManifold m_manifold;
	//b2Fixture*m_groundFixture;

	bool m_onGround;
	float m_jumpSpeed;
	float m_runSpeed;

	void setupBody(glm::vec2,glm::vec2);
public:
	TerrestrialBody(glm::vec2 pos, glm::vec2 dimension);
	~TerrestrialBody();

	void Update(float deltaTime) override;
	void HandleBeginContact(b2Contact*contact, b2Fixture*fixture)override;
	inline b2Body*GetBody()override { return m_body; }



	void Stop();
	void Run(bool left_right);//true is right, false is left
	void Jump();
	void JumpAndRun(bool left_right);

	bool IsOnGround();

	inline Leg& GetLeg() { return m_leg; }
};
