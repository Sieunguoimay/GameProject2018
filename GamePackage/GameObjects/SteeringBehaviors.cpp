#include"SteeringBehaviors.h"
#include"../misc/GameAccessor.h"
#include"../misc/Assistances.h"
SteeringBehaviors::SteeringBehaviors(MovingObject*owner)
	:m_pOwner(owner), 
	m_wanderRadius(5),
	m_wanderDistance(2),
	m_wanderJitter(200.0),
	m_waypointSeekDistSq(400.0f),
	m_minBoxLength(200.0f),
	m_neighborRadius(500.0f)
{
}
SteeringBehaviors::~SteeringBehaviors() {
	m_pOwner = NULL;
}

b2Vec2 SteeringBehaviors::Calculate(float deltaTime) {
	b2Vec2 steeringForce(0.0f, 0.0f);

	if (m_pOwner == NULL)//stop babe
		return steeringForce;

	if (On(seek)) {
		steeringForce += Seek(m_pOwner->m_target);
	}
	if (On(arrive)) {
		steeringForce += Arrive(m_pOwner->m_target, fast);
	}
	if (On(pursuit)) {
		steeringForce += Pursuit(m_pOwner->m_pEvader);
	}
	if (On(wander)) {
		steeringForce += Wander(deltaTime);
	}
	if (On(obstacle_avoidance)) {
		steeringForce += ObstacleAvoidance(*m_pObstacles);
	}
	if (On(interpose)) {
		steeringForce += Interpose(m_pOwner->m_pAgentA, m_pOwner->m_pAgentB);
	}
	if (On(hide)) {
		steeringForce += Hide(m_pOwner->m_pPursuer, *m_pObstacles);
	}
	if (On(follow_path)) {
		steeringForce += FollowPath();
	}
	if (On(offset_pursuit)) {
		steeringForce += OffsetPursuit(m_pOwner->m_pLeader, m_pOwner->m_offset);
	}

	if (On(seperation) || On(alignment) || On(cohesion)) {
		TagNeighbors(m_pFlock);
	}
	if (On(seperation)) {
		steeringForce += 0.5f*Seperation(m_pOwner->m_neighbors);
	}
	if (On(alignment)) {
		steeringForce += 0.6f*Alignment(m_pOwner->m_neighbors);
	}
	if (On(cohesion)) {
		steeringForce += 0.2f*Cohesion(m_pOwner->m_neighbors);
	}

	return steeringForce;
}
b2Vec2 SteeringBehaviors::Seek(b2Vec2 targetPos) {
	b2Vec2 desired = targetPos - m_pOwner->GetPos();
	desired.Normalize();
	desired *= m_pOwner->GetMaxSpeed();

	return CalculateForceFromDesiredVel(desired);
}

b2Vec2 SteeringBehaviors::Flee(b2Vec2 targetPos)
{
	b2Vec2 desired = m_pOwner->GetPos()-targetPos;
	float distanceSq = desired.LengthSquared();
	const float panicDistanceSq = 100.0f*100.0f;
	if (distanceSq > panicDistanceSq) {
		return b2Vec2(0.0f, 0.0f);
	}

	desired.Normalize();
	desired *= m_pOwner->GetMaxSpeed();

	return CalculateForceFromDesiredVel(desired);
}

b2Vec2 SteeringBehaviors::Arrive(b2Vec2 targetPos, Deceleration deceleration)
{
	b2Vec2 toTarget = targetPos - m_pOwner->GetPos();
	float distance = toTarget.Length();
	if (distance > 0.0001f) {
		float speed = distance / ((float)deceleration*0.3f);
		speed = b2Min(speed, m_pOwner->GetMaxSpeed());
		b2Vec2 desired = (speed / distance)*toTarget;

		return CalculateForceFromDesiredVel(desired);
	}
	return b2Vec2(0.0f,0.0f);
}

b2Vec2 SteeringBehaviors::Pursuit(MovingObject * evader)
{
	if(evader==NULL)
		return b2Vec2(0.0f,0.0f);

	b2Vec2 toEvader = evader->GetPos() - m_pOwner->GetPos();
	float relativeHeading = b2Dot(m_pOwner->GetHeading(),evader->GetHeading());
	if (b2Dot(toEvader, m_pOwner->GetHeading()) > 0
		&&relativeHeading<-0.95f) //acos(0.95)==18 degs
	{
		return Seek(evader->GetPos());
	}
	float lookAheadTime = toEvader.Length() / (m_pOwner->GetMaxSpeed() + evader->GetVel().Length());
	return Seek(evader->GetPos() + lookAheadTime*evader->GetVel());
}

b2Vec2 SteeringBehaviors::Evade(MovingObject * pursuer)
{
	if (pursuer== NULL)
		return b2Vec2(0.0f, 0.0f);

	b2Vec2 toPursuer= pursuer->GetPos() - m_pOwner->GetPos();
	float relativeHeading = b2Dot(m_pOwner->GetHeading(), pursuer->GetHeading());
	if (b2Dot(toPursuer, m_pOwner->GetHeading()) > 0
		&& relativeHeading<-0.95f) //acos(0.95)==18 degs
	{
		return Flee(pursuer->GetPos());
	}
	float lookAheadTime = toPursuer.Length() / (m_pOwner->GetMaxSpeed() + pursuer->GetVel().Length());
	return Flee(pursuer->GetPos() + lookAheadTime*pursuer->GetVel());
}

b2Vec2 SteeringBehaviors::Wander(float deltaTime)
{
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	float JitterThisTimeSlice = m_wanderJitter * deltaTime;

	//first, add a small random vector to the target's position
	m_pOwner->m_wanderTarget += b2Vec2(RandomClamped() * JitterThisTimeSlice,
		RandomClamped() * JitterThisTimeSlice);

	//reproject this new vector back on to a unit circle
	m_pOwner->m_wanderTarget.Normalize();

	//increase the length of the vector to the same as the radius
	//of the wander circle
	m_pOwner->m_wanderTarget *= m_wanderRadius;

	//move the target into a position WanderDist in front of the agent
	b2Vec2 target = m_pOwner->m_wanderTarget + b2Vec2(m_wanderDistance, 0.0f);

	//project the target into world space
	b2Vec2 Target = PointToWorldSpace(target,
		m_pOwner->GetHeading(),
		m_pOwner->GetSide(),
		m_pOwner->GetPos());
	//SDL_Log("%f %f", target.x, target.y);

	//and steer towards it
	return Seek(Target);
}

b2Vec2 SteeringBehaviors::ObstacleAvoidance(const std::vector<BaseObject*>& obstacles)
{
	//the detection box length is proportional to the agent's velocity
	m_boxLength = m_minBoxLength +(m_pOwner->GetVel().Length() / m_pOwner->GetMaxSpeed()) *m_minBoxLength;
	float distanceToClosestIP = FLT_MAX;
	BaseObject*ClosestIntersectingObstacle = NULL;
	b2Vec2 LocalPosOfClosestObstacle;

	for (auto&obstacle : obstacles) {
		if (obstacle == m_pOwner)continue;
		float distanceSq = (obstacle->GetPos() - m_pOwner->GetPos()).LengthSquared();
		if (distanceSq < m_boxLength*m_boxLength) {
			b2Vec2 LocalPos = PointToLocalSpace(obstacle->GetPos(),
				m_pOwner->GetHeading(),
				m_pOwner->GetSide(),
				m_pOwner->GetPos());
			if (LocalPos.x >= 0) {
				float ExpandedRadius = obstacle->GetBoundRadius() + m_pOwner->GetBoundRadius();
				if (fabs(LocalPos.y) < ExpandedRadius)
				{
					float cX = LocalPos.x;
					float cY = LocalPos.y;

					float SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);

					float ip = cX - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cX + SqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < distanceToClosestIP)
					{
						distanceToClosestIP = ip;

						ClosestIntersectingObstacle = obstacle;

						LocalPosOfClosestObstacle = LocalPos;
	
					}
				}

			}
		}
	}
	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	b2Vec2 SteeringForce;

	if (ClosestIntersectingObstacle!=NULL)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		float multiplier = 1.0 + (m_boxLength - LocalPosOfClosestObstacle.x)/m_boxLength;

		//calculate the lateral force
		SteeringForce.y = (ClosestIntersectingObstacle->GetBoundRadius() - LocalPosOfClosestObstacle.y)  * multiplier;

		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const float BrakingWeight = 0.02;

		SteeringForce.x = (ClosestIntersectingObstacle->GetBoundRadius() - LocalPosOfClosestObstacle.x) *BrakingWeight;

		b2Vec2 desired = VectorToWorldSpace(SteeringForce,
			m_pOwner->GetHeading(),
			m_pOwner->GetSide());


		desired.Normalize();
		desired *= m_pOwner->GetMaxSpeed();

		b2Vec2 velChange = desired - m_pOwner->GetVel();
		b2Vec2 force = m_pOwner->GetPhysicsBody()->GetBody()->GetMass()*(1.0f / 20.0f)*velChange;
		//SDL_Log("%f %f", SteeringForce.x, SteeringForce.y);
		return force;
	}

	//finally, convert the steering vector from local to world space

	return b2Vec2(0.0f,0.0f);
}

b2Vec2 SteeringBehaviors::Interpose(MovingObject * agentA, MovingObject * agentB)
{
	if (agentA != NULL&&agentB != NULL) {
		b2Vec2 currentMidPoint = 0.5f*(agentA->GetPos() + agentB->GetPos());
		float timeToReachMidPoint = m_pOwner->GetMaxSpeed()*(currentMidPoint - m_pOwner->GetPos()).Length();

		b2Vec2 futurePosA = agentA->GetPos()+timeToReachMidPoint*agentA->GetVel();
		b2Vec2 futurePosB = agentB->GetPos() + timeToReachMidPoint*agentB->GetVel();
		b2Vec2 furtureMidPoint = 0.5f*(futurePosA + futurePosB);
		return Arrive(furtureMidPoint, fast);
	}
	return b2Vec2(0.0f, 0.0f);
}

b2Vec2 SteeringBehaviors::GetHidingPosition(const b2Vec2 & obstaclePos, const float obstacleRadius, const b2Vec2 & targetPos)
{
	const float distanceFromBoundary = m_pOwner->GetBoundRadius();
	float distanceFromObstaclePos = distanceFromBoundary + obstacleRadius;
	b2Vec2 targetToObstacle = obstaclePos - targetPos;
	targetToObstacle.Normalize();
	return obstaclePos+distanceFromObstaclePos*targetToObstacle;
}

b2Vec2 SteeringBehaviors::Hide(MovingObject * target, const std::vector<BaseObject*>& obstacles)
{
	if(target == m_pOwner)
		return b2Vec2(0.0f, 0.0f);

	float closestDistance = FLT_MAX;
	b2Vec2 closestHidingPos;
	for (auto&obstacle : obstacles) {
		if (obstacle == m_pOwner)continue;
		if (obstacle == target)continue;
		b2Vec2 hidingPos = GetHidingPosition(obstacle->GetPos(), obstacle->GetBoundRadius(), target->GetPos());
		float distanceSq = (hidingPos - m_pOwner->GetPos()).LengthSquared();
		if (distanceSq < closestDistance) {
			closestDistance = distanceSq;
			closestHidingPos = hidingPos;
		}
	}
	if (closestDistance != FLT_MAX) {
		return Arrive(closestHidingPos,fast);
	}
	return b2Vec2(0.0f,0.0f);
}

b2Vec2 SteeringBehaviors::FollowPath()
{
	if (m_pOwner->m_pPath == NULL|| m_pOwner->m_pPath->empty())
		return b2Vec2(0.0f, 0.0f);
	if (m_pOwner->m_currentWaypoint < m_pOwner->m_pPath->size() - 1)
		if ((m_pOwner->m_pPath->at(m_pOwner->m_currentWaypoint) - m_pOwner->GetPos()).LengthSquared()<m_waypointSeekDistSq)
			m_pOwner->m_currentWaypoint=(m_pOwner->m_currentWaypoint+1);
	
	if (m_pOwner->m_currentWaypoint == m_pOwner->m_pPath->size() - 1)
		return Arrive(m_pOwner->m_pPath->at(m_pOwner->m_currentWaypoint),normal);
	else
		return Seek(m_pOwner->m_pPath->at(m_pOwner->m_currentWaypoint));
}

b2Vec2 SteeringBehaviors::OffsetPursuit(MovingObject * leader, const b2Vec2 offset)
{
	if (leader == NULL)
		return b2Vec2(0.0,0.0);
	b2Vec2 offsetPos = PointToWorldSpace(offset, leader->GetHeading(), leader->GetSide(), leader->GetPos());
	b2Vec2 toOffsetPos = offsetPos - m_pOwner->GetPos();
	float lookAheadTime = toOffsetPos.Length() / (m_pOwner->GetMaxSpeed() + leader->GetVel().Length());
	return Arrive(offsetPos+lookAheadTime*leader->GetVel(),fast);
}

void SteeringBehaviors::TagNeighbors(const std::vector<MovingObject*>*movingObjects)
{
	if (movingObjects == NULL)return;
	if(!m_pOwner->m_neighbors.empty())
		m_pOwner->m_neighbors.clear();
	for (auto&object : *movingObjects) {
		if (object == m_pOwner) continue;
		b2Vec2 toMovingObject = object->GetPos() - m_pOwner->GetPos();
		float range = m_neighborRadius + object->GetBoundRadius();
		if (toMovingObject.LengthSquared() < range*range) {
			m_pOwner->m_neighbors.push_back(object);
		}
	}
}

b2Vec2 SteeringBehaviors::Seperation(const std::vector<MovingObject*>& neighbors)
{
	float desiredSeparation = m_pOwner->GetBoundRadius()*2.0f;
	b2Vec2 steeringForce(0.0f,0.0f);
	int count = 0;
	for (auto&neighbor: neighbors) {
		b2Vec2 toNeighbor = m_pOwner->GetPos() - neighbor->GetPos();
		float distance = toNeighbor.Length();
		if (distance < desiredSeparation) {
			toNeighbor.Normalize();
			steeringForce += (1.0f / distance)*toNeighbor;
			count++;
		}
	}
	if (count > 0) {
		steeringForce *= (1.0f / (float)count);
		steeringForce.Normalize();
		steeringForce *= m_pOwner->GetMaxSpeed();
		return CalculateForceFromDesiredVel(steeringForce);
	}
	return b2Vec2(0.0f, 0.0f);
}

b2Vec2 SteeringBehaviors::Alignment(const std::vector<MovingObject*>& neighbors)
{
	if (neighbors.empty())
		return b2Vec2(0.0f, 0.0f);

	b2Vec2 averageHeading(0.0f, 0.0f);
	for (auto&neighbor : neighbors) 
		averageHeading += neighbor->GetVel();
	averageHeading *= 1.0f/(float)neighbors.size();
	b2Vec2 desired = averageHeading;
	desired.Normalize();
	desired *= m_pOwner->GetMaxSpeed();
	b2Vec2 steer = desired - m_pOwner->GetVel();
	return CalculateForceFromDesiredVel(steer);
}

b2Vec2 SteeringBehaviors::Cohesion(const std::vector<MovingObject*>& neighbors)
{
	if (neighbors.empty())
		return b2Vec2(0.0f, 0.0f);
	b2Vec2 centerOfMass(0.0f, 0.0f);
	for (auto&neighbor : neighbors)
		centerOfMass += neighbor->GetPos();
	centerOfMass *= 1.0f / (float)neighbors.size();
	return Seek(centerOfMass);
}

b2Vec2 SteeringBehaviors::CalculateForceFromDesiredVel(b2Vec2 desired)
{
	b2Vec2 velChange = desired - m_pOwner->GetVel();
	b2Vec2 force = m_pOwner->GetPhysicsBody()->GetBody()->GetMass()*(1.0f / 20.0f)*velChange;
	return force;
}
