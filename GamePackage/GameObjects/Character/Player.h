#pragma once
#include<vector>
#include"../Box.h"
#include"../Entity.h"
#include"../../misc/StateMachine/StateMachine.h"
#include"../../2D/AnimationCollection.h"

class Capsule {
	b2Body*m_body;
	b2Fixture*m_fixtures[2];
	glm::vec2 m_dimension;
	glm::vec2 m_position;
public:
	~Capsule();
	Capsule(b2World*world, const glm::vec2& pos, const glm::vec2& collisionDimension, 
		float density, float friction, bool fixedRotation = false);
	void Update(float deltaTime);
	void Draw();


	b2Body*GetBody()const { return m_body; }
	b2Fixture*GetFixture(int index)const { return m_fixtures[index]; }
	const glm::vec2& GetPosition() { return m_position; }
	const glm::vec2& GetDimension()const { return m_dimension; }
};



class Player;
class PlayerBehavior {
public:
	void Update(Player*player, float deltaTime);
};
enum PlayerStates {
	STANDING_STATE,
	IN_THE_AIR_STATE,
	RUNNING_STATE
};




class StandingState:public State<Player> {
public:
	void Enter(Player*player)override;
	void Execute(Player*player,float deltaTime)override;
	void Exit(Player*player) override;
};
class InTheAirState :public State<Player> {
	float m_during;
public:
	void Enter(Player*player)override;
	void Execute(Player*player, float deltaTime)override;
	void Exit(Player*player) override;
};
class WalkingState :public State<Player> {
public:
	void Enter(Player*player)override;
	void Execute(Player*player, float deltaTime)override;
	void Exit(Player*player) override;
};




class Player:public Entity
{
	//graphics
	//Texture*m_pTexture;
	AnimationCollection*m_animation;
	//physics
	Capsule m_capsule;

	//behavior
	PlayerBehavior m_behavior;

	glm::vec2 m_drawDimension;
	glm::vec2 m_position;

	//states
	StateMachine<Player>m_stateMachine;
	std::vector<State<Player>*>m_states;

	void checkOnGround();
	bool m_onGround = false;
	float m_runSpeed;
	float m_jumpSpeed;
public:
	Player(b2World*world, const glm::vec2& pos, const glm::vec2& collisionDimension, const glm::vec2& drawDimension, AnimationCollection*animationCollection);
	~Player();

	void Update(float deltaTime)override;
	void Draw()override;

	Capsule&GetCapsule() { return m_capsule; }
	State<Player>*GetState(int index) { return m_states[index]; }
	StateMachine<Player>&GetStateMachine() { return m_stateMachine; }
	AnimationCollection*GetAnimationCollection() { return m_animation; }

	bool IsOnGround() { return m_onGround; }
	float GetRunSpeed() { return m_runSpeed; }
	float GetJumpSpeed() { return m_jumpSpeed; }

};

