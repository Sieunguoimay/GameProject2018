#pragma once
#include"../../PhysicsEngine/BodyBase.h"
#include"../TouchPoint.h"
#include"ActionStates.h"
#include"Percept.h"

constexpr auto RUN_SPEED = 2.0f;
constexpr auto WALK_SPEED = 1.0f;



class PlayerActuator
	: public StateMachine<PlayerActuator> {


	enum HandStates {
		AT_REST,
		HOLD_OBJECT,
		DONE
	};
	int m_handState;

	float m_jumpSpeed;
	float m_runSpeed;

	b2DistanceJoint*m_joint = NULL;

	//some pointers
	class Player*m_pOwner;

	void _Stop();
	void _Move(bool left_right, float speed = RUN_SPEED);//true is right, false is left
	void _Jump();
	void _JumpAndRun(bool left_right);

	void HoldTouchPoint(Percept*perceptTouchPoint);
	void LeaveTouchPoint();
	
	BaseActionState* m_actionStates[ActionStateEnum::AS_TOTAL_NUM];
	int m_currentActionState;

public:
	PlayerActuator(Player*pOwner);
	~PlayerActuator();

	//how to install me
	void Update(float deltaTime);

	BaseActionState* ChangeStateByIndex(int index,bool setAnimationOnEnterFlag = true);
	bool CheckInState(int index) { return index == m_currentActionState; }
	bool CheckInStates(int index, ...);

	//primitive actions
	void RunRight();
	void RunLeft();
	void Jump();
	void JumpLeft();
	void JumpRight();
	void PushLeft();
	void PushRight();
	void PullLeft(Percept*perceptTouchPoint);
	void PullRight(Percept*perceptTouchPoint);
	void ClimbOnRock(Percept*percept);
	void ClimbDownRockLeft(Percept*percept);
	void ClimbDownRockRight(Percept*percept);
	void Stop();
	void Stand();

	void RaiseHands(Percept*percept,float deltaTime);

	friend class BaseActionState;
	friend class _GlobalCharacterState;
	friend class _JumpingState;
	friend class _JumpingBufferState;
	friend class _ClimbingState;
	friend class _StoppingState;
};







namespace pa {

class PlayerLongActionBase :public pe::BaseAction {
	void Run(float deltaTime) override {}
public:
	virtual void Enter(){}
	virtual void Run(float deltaTime, Percept*percept){}
	virtual void Exit() {}
};

class LongActionMoveUntilTouchRock :public PlayerLongActionBase {
	bool m_done;
	b2Vec2 m_vel;
	Player*m_pOwner;
public:
	LongActionMoveUntilTouchRock(Player*pOwner):m_pOwner(pOwner){}
	~LongActionMoveUntilTouchRock() { }
	void Init(const b2Vec2& vel) {m_vel = vel;m_done = false;}
	void Run(float deltaTime, Percept*percept)override;
	bool Timeout() override { return m_done; }
};

class LongActionClimbOnRock :public PlayerLongActionBase {
	bool m_done;
	Player*m_pOwner;
public:
	LongActionClimbOnRock(Player*pOwner) :m_pOwner(pOwner){}
	void Enter()override;
	void Exit() override;
	bool Timeout() override { return m_done; }
};

class LongActionClimbDownRock :public PlayerLongActionBase {
	bool m_done;
	Player*m_pOwner;
	int m_state = AS_CLIMB_DOWN_LEFT;
public:
	LongActionClimbDownRock(Player*pOwner, int state) :m_pOwner(pOwner),m_state(state) {}
	void Enter()override;
	void Run(float deltaTime, Percept*percept)override;
	void Exit() override;
	bool Timeout() override { return m_done; }
};

class LongActionDelay :public PlayerLongActionBase {
	float m_time;
	std::function<void(float)>m_callback;
public:
	LongActionDelay(float time, std::function<void(float)>callback) 
		:m_time(time), m_callback(callback) {}
	void Run(float deltaTime, Percept*percept) override 
		{ m_time -= deltaTime; if(m_callback!=NULL) m_callback(deltaTime); }
	bool Timeout() override { return (m_time <= 0.0f); }
};

class ActionCallback :public PlayerLongActionBase {
	std::function<void()>m_callback;
public:
	ActionCallback(std::function<void()>callback):m_callback(callback){}
	void Run(float deltaTime, Percept*percept)override;
};
}
