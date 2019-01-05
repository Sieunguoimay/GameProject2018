#pragma once
#include"BodyKit.h"

class TerrestrialBody:public BodyBase {

	bool m_onGround;
	float m_jumpSpeed;
	float m_runSpeed;

public:
	TerrestrialBody(b2World*world, glm::vec2 pos, glm::vec2 dimension);
	~TerrestrialBody();

	void Update(float deltaTime) override;


	void Stop();
	void Run(bool left_right);//true is right, false is left
	void Jump();
	void JumpAndRun(bool left_right);

	bool IsOnGround();

	
};
