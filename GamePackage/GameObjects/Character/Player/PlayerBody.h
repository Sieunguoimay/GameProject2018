#pragma once
#include"../../PhysicsEngine/BodyBase.h"
#include"../TouchPoint.h"
#include"ActionStates.h"

#define RUN_SPEED 2.0f
#define WALK_SPEED 1.3f

class PlayerBodyCallback {
public:
	virtual ~PlayerBodyCallback(){}
	virtual void OnStateChanged(int brainState) = 0;
};


class PlayerBody
	:public b2RayCastCallback
	, public b2QueryCallback
	, public BodyBase
	, public StateMachine<PlayerBody> {
	
	enum TouchEdge{
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


	enum BodyStates {
		BODY_STATE_ON_THE_FLOOR,
		BODY_STATE_ON_THE_FLOOR_2,
		BODY_STATE_NEXT_TO_ROCK_LHS,
		BODY_STATE_NEXT_TO_ROCK_LHS_2,
		BODY_STATE_NEXT_TO_ROCK_RHS,
		BODY_STATE_NEXT_TO_ROCK_RHS_2,
		BODY_STATE_ON_ROCK_TOP_LEFT,
		BODY_STATE_ON_ROCK_TOP_LEFT_2,
		BODY_STATE_ON_ROCK_TOP_RIGHT,
		BODY_STATE_ON_ROCK_TOP_RIGHT_2
	};
	int m_bodyState;
	void signalState();

	enum HandStates {
		AT_REST,
		HOLD_OBJECT,
		DONE
	};
	int m_handState;


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

	//Radar for hand control AABB query
	void updateAABBRadar();
	bool ReportFixture(b2Fixture* fixture)override;
	bool m_rockOnRadar;
	b2AABB m_radarAABB;
	Node<b2Vec2>*m_iterator;
	List<b2Vec2> m_terrain;
	std::vector<TouchPoint*>m_pTouchPoints;
	TouchPoint*m_nearestTouchPoint;
	b2DistanceJoint*m_joint = NULL;

	b2Vec2 m_dir;

	//some pointers
	class PlayerSkin*m_pSkin;
	PlayerBodyCallback*m_pCallback;

	BaseActionState* m_actionStates[ActionStateEnum::AS_TOTAL_NUM];
	int m_currentActionState;

	//provide information about me
	inline const b2Vec2& GetGroundNormal()const { return m_groundNormal; }
	inline const float& GetGroundNormalAngle()const { return m_normalAngle; }
	inline bool IsOnGround() const { return m_touchEdges[BOTTOM]; }

	void _Stop();
	void _Move(bool left_right, float speed = RUN_SPEED);//true is right, false is left
	void _Jump();
	void _JumpAndRun(bool left_right);

	void calculateNearestTouchPoint();
	inline TouchPoint*GetNearestTouchPoint(){ return m_nearestTouchPoint; }
	inline void ChangeStateByIndex(int index) { ChangeState(m_actionStates[index]); m_currentActionState = index; }
	void HoldTouchPoint(const b2Vec2&point);
	void LeaveTouchPoint();

public:
	PlayerBody(PlayerSkin*pSkin, PlayerBodyCallback*pCallback);
	~PlayerBody();

	//how to install me
	b2Body*SetupBody(glm::vec2, glm::vec2);
	void Update(float deltaTime);

	inline bool CheckInState(int index) { return index == m_currentActionState; }
	bool CheckInStates(int index, ...);

	//primitive actions
	void RunRight();
	void RunLeft();
	void Jump();
	void JumpLeft();
	void JumpRight();
	void PushLeft();
	void PushRight();
	void PullLeft();
	void PullRight();
	void ClimbOnLeft();
	void ClimbOnRight();
	void Stop();
	void Stand();

	friend class BaseActionState;
	friend class _GlobalCharacterState;
	friend class _JumpingState;
	friend class _JumpingBufferState;
	friend class _StoppingState;
};
