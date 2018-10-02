#pragma once
#include"MovingObject.h"
enum Deceleration {
	slow = 3,
	normal = 2,
	fast = 1
};
enum Flags {
	none = 0,
	seek = 1,
	flee = 2,
	arrive = 4,
	pursuit = 8,
	evade = 16,
	wander = 32,
	obstacle_avoidance = 64,
	wall_avoidance = 128,
	interpose = 256,
	hide = 512,
	follow_path = 1024,
	offset_pursuit = 2048,
	seperation = 4096,
	alignment = 8192,
	cohesion = 16384
};

class SteeringBehaviors {

	//this owner can be switched.
	MovingObject*m_pOwner;


	//for wandering
	//this thing can be used as common
	float m_wanderRadius;
	float m_wanderDistance;
	float m_wanderJitter;

	//obstacle avoidance -> currently no problem
	float m_boxLength;
	float m_minBoxLength;

	//used as common
	std::vector<BaseObject*>*m_pObstacles;

	//path following
	float m_waypointSeekDistSq;

	//group behaviors
	float m_neighborRadius;
	std::vector<MovingObject*>*m_pFlock;





	//just a function ok
	bool On(Flags type) { return ((m_pOwner->m_flags&type) == type); }
	b2Vec2 CalculateForceFromDesiredVel(b2Vec2 desired);

public:
	SteeringBehaviors(MovingObject*owner);
	~SteeringBehaviors();

	b2Vec2 Calculate(float deltaTime);

	b2Vec2 Seek(b2Vec2 targetPos);
	b2Vec2 Flee(b2Vec2 targetPos);//opposite of Seek()
	b2Vec2 Arrive(b2Vec2 targetPos, Deceleration deceleration);
	b2Vec2 Pursuit(MovingObject*evader);//seek the furture pos
	b2Vec2 Evade(MovingObject*pursuer);//flee from the furture pos i.e pos+vel
	b2Vec2 Wander(float deltaTime);
	b2Vec2 ObstacleAvoidance(const std::vector<BaseObject*>&obstacles);
	b2Vec2 Interpose(MovingObject*agentA, MovingObject*agentB);
	b2Vec2 GetHidingPosition(const b2Vec2&posOb, const float radiusOb, const b2Vec2&posTarget);
	b2Vec2 Hide(MovingObject*target, const std::vector<BaseObject*>&obstacles);
	b2Vec2 FollowPath();
	b2Vec2 OffsetPursuit(MovingObject* leader, const b2Vec2 offset);
	void TagNeighbors(const std::vector<MovingObject*>*movingObjects);
	b2Vec2 Seperation(const std::vector<MovingObject*>&neighbors);
	b2Vec2 Alignment(const std::vector<MovingObject*>&neighbors);
	b2Vec2 Cohesion(const std::vector<MovingObject*>&neighbors);


	void SetEvader(MovingObject*evader) { m_pOwner->m_pEvader = evader; }
	void SetPursuer(MovingObject*pursuer) { m_pOwner->m_pPursuer= pursuer; }
	void SetInterposeAgents(MovingObject*agentA, MovingObject*agentB) { m_pOwner->m_pAgentA = agentA; m_pOwner->m_pAgentB = agentB; }
	void SetPath(std::vector<b2Vec2>*path) { m_pOwner->m_pPath = path; }
	void SetWaypoint(int i) { if (m_pOwner->m_pPath&&i < m_pOwner->m_pPath->size())m_pOwner->m_currentWaypoint = i; }
	void SetOffsetPursuit(MovingObject* leader, const b2Vec2 offset) { if (m_pOwner->m_pLeader == m_pOwner) m_pOwner->m_pLeader = NULL; else { m_pOwner->m_pLeader = leader; m_pOwner->m_offset = offset; } }
	void SetTarget(b2Vec2 target) { this->m_pOwner->m_target = target; }

	void SetOwner(MovingObject*owner) { this->m_pOwner = owner; }
	void SetObstacles(std::vector<BaseObject*>*obstacles) { this->m_pObstacles = obstacles; }
	void SetWaypointSeekDistSq(float distSq) { m_waypointSeekDistSq = distSq; }
	void SetFlock(std::vector<MovingObject*>*flock) { m_pFlock = flock; }


	void SeekOn() { m_pOwner->m_flags |= Flags::seek; }
	void FleeOn() { m_pOwner->m_flags |= Flags::flee; }
	void ArriveOn() { m_pOwner->m_flags |= Flags::arrive; }
	void PursuitOn() { m_pOwner->m_flags |= Flags::pursuit; }
	void EvadeOn() { m_pOwner->m_flags |= Flags::evade; }
	void WanderOn() { m_pOwner->m_flags |= Flags::wander; }
	void ObstacleAvoidanceOn() { m_pOwner->m_flags |= Flags::obstacle_avoidance; }
	void WallAvoidanceOn() { m_pOwner->m_flags |= Flags::wall_avoidance; }
	void InterposeOn() { m_pOwner->m_flags |= Flags::interpose; }
	void HideOn() { m_pOwner->m_flags |= Flags::hide; }
	void FollowPathOn() { m_pOwner->m_flags |= Flags::follow_path; }
	void OffsetPursuitOn() { m_pOwner->m_flags |= Flags::offset_pursuit; }
	void SeperationOn() { m_pOwner->m_flags |= Flags::seperation; }
	void AlignmentOn() { m_pOwner->m_flags |= Flags::alignment; }
	void CohesionOn() { m_pOwner->m_flags |= Flags::cohesion; }


	void SeekOff() { m_pOwner->m_flags ^= Flags::seek; }
	void FleeOff() { m_pOwner->m_flags ^= Flags::flee; }
	void ArriveOff() { m_pOwner->m_flags ^= Flags::arrive; }
	void PursuitOff() { m_pOwner->m_flags ^= Flags::pursuit; }
	void EvadeOff() { m_pOwner->m_flags ^= Flags::evade; }
	void WanderOff() { m_pOwner->m_flags ^= Flags::wander; }
	void ObstacleAvoidanceOff() { m_pOwner->m_flags ^= Flags::obstacle_avoidance; }
	void WallAvoidanceOff() { m_pOwner->m_flags ^= Flags::wall_avoidance; }
	void InterposeOff() { m_pOwner->m_flags ^= Flags::interpose; }
	void HideOff() { m_pOwner->m_flags ^= Flags::hide; }
	void FollowPathOff() { m_pOwner->m_flags ^= Flags::follow_path; }
	void OffsetPursuitOff() { m_pOwner->m_flags ^= Flags::offset_pursuit; }
	void SeperationOff() { m_pOwner->m_flags ^= Flags::seperation; }
	void AlignmentOff() { m_pOwner->m_flags ^= Flags::alignment; }
	void CohesionOff() { m_pOwner->m_flags ^= Flags::cohesion; }

};
