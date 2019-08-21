#include "PlayerBody.h"
#include"../../../misc/Assistances.h"
#include"../../../misc/Locator.h"
#include"../../../misc/Math/Geometry.h"
#include"../../EntityHierachy/HavingBodyEntity.h"
#include"BrainStates.h"
#include"PlayerSkin.h"

b2Body*PlayerBody::SetupBody(glm::vec2 pos,glm::vec2 dimension)
{
	//Create the circles
	b2CircleShape cs;
	cs.m_radius = dimension.x / (2.0f*M2P);

	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / (2.0f*M2P));
	m_body = Locator::GetPhysicsFactory()->CreateBody(&cs, b2_dynamicBody, MEAT, b2Vec2(pos.x / M2P, pos.y / M2P));

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / (2.0f*M2P));
	m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(MEAT));

	//sensor shape

	//cs.m_radius = (dimension.x+10.0f) / M2P;
	//cs.m_p.Set(0.0f, 0.0f);
	//m_body->CreateFixture(&Locator::GetPhysicsFactory()->GetFixture(SENSOR,&cs));

	m_body->SetFixedRotation(true);
	m_body->SetUserData(m_bodyBaseThis);
	m_bodyId = BodyObjectType::BOT_PLAYER;
	//m_leg.Init(body);

	SDL_Log("Player's Mass: %f", m_body->GetMass());
	return m_body;
}

PlayerBody::PlayerBody(PlayerSkin*pSkin, PlayerBodyCallback*pCallback)
	: m_jumpSpeed(3.50f)
	, m_runSpeed(RUN_SPEED)
	, m_pSkin(pSkin)
	, m_pCallback(pCallback)
{
	m_bodyState = BODY_STATE_ON_THE_FLOOR;
	m_handState = HandStates::AT_REST;
	m_currentActionState = AS_STAND;

	int runIndex =		m_pSkin->GetSpriterEntity()->GetAnimationIndex("run");
	int jumpIndex =		m_pSkin->GetSpriterEntity()->GetAnimationIndex("jump");
	int fallIndex =		m_pSkin->GetSpriterEntity()->GetAnimationIndex("fall");
	int standIndex =	m_pSkin->GetSpriterEntity()->GetAnimationIndex("stand");
	int stopIndex =		m_pSkin->GetSpriterEntity()->GetAnimationIndex("stop");
	int jump2Index =	m_pSkin->GetSpriterEntity()->GetAnimationIndex("jump2");
	int pullIndex =		m_pSkin->GetSpriterEntity()->GetAnimationIndex("walk_backward");

	(m_actionStates[AS_RUN_LEFT] =			new BaseActionState(runIndex)	)->SetName("Run left");
	(m_actionStates[AS_RUN_RIGHT] =			new BaseActionState(runIndex)	)->SetName("Run right");
	(m_actionStates[AS_JUMP] =				new _JumpingState())->SetAnimationIndex(jumpIndex)->SetName("Jump");
	(m_actionStates[AS_FALL] =				new BaseActionState(fallIndex)	)->SetName("Fall");
	(m_actionStates[AS_JUMP_LEFT] =			new BaseActionState(jumpIndex)	)->SetName("Jump left");
	(m_actionStates[AS_JUMP_RIGHT] =		new BaseActionState(jumpIndex)	)->SetName("Jump right");
	m_actionStates[AS_PUSH_LEFT] =			new BaseActionState(runIndex);
	m_actionStates[AS_PUSH_RIGHT] =			new BaseActionState(runIndex);
	m_actionStates[AS_PULL_LEFT] =			new BaseActionState(pullIndex);
	m_actionStates[AS_PULL_RIGHT] =			new BaseActionState(pullIndex);
	m_actionStates[AS_CLIMB_ON_LEFT] =		new BaseActionState(standIndex);
	m_actionStates[AS_CLIMB_ON_RIGHT] =		new BaseActionState(standIndex);
	(m_actionStates[AS_STOP] =				new _StoppingState())->SetAnimationIndex(stopIndex)->SetName("Stop");
	(m_actionStates[AS_STAND] =				new BaseActionState(standIndex)	)->SetName("Stand");
	(m_actionStates[AS_GLOBAL_STATE] =		new _GlobalCharacterState()		)->SetName("Global State");

	StateMachine<PlayerBody>::Init(this, m_actionStates[AS_STAND], NULL, m_actionStates[AS_GLOBAL_STATE]);
	SDL_Log("Created PlayerBody");
}

PlayerBody::~PlayerBody()
{
	m_pCallback = NULL;
	m_pSkin = NULL;
	m_body = NULL;
	for (int i = 0; i < AS_TOTAL_NUM; i++) delete m_actionStates[i];

	SDL_Log("Destroyed PlayerBody");
}

void PlayerBody::Update(float deltaTime)
{
	StateMachine<PlayerBody>::Update(deltaTime);

	//work with normal Ground angle
	if (m_newGroundNormal) {
		//calculate pelvis angle -> costly
		m_normalAngle = glm::degrees(b2Atan2(m_groundNormal.y, m_groundNormal.x));
		if (m_normalAngle< 0)m_normalAngle += 360.0f;
		else if (m_normalAngle >= 360.0f)m_normalAngle -= 360.0f;
		m_newGroundNormal = false;
	}

	//ray cast
	b2Vec2 p1 = m_body->GetPosition();
	b2Vec2 p2 = p1 + b2Vec2(0.0, -2.0f);
	m_minRayCastFraction = 1.0f;//initial
	Locator::GetPhysicsFactory()->GetB2World()->RayCast(this, p1, p2);


	updateAABBRadar();


	//reset
	//if (m_body->GetPosition().y < -500.0f/M2P) {
	//	Locator::GetPhysicsFactory()->GetB2World()->DestroyBody(m_body);
	//	SetupBody(glm::vec2(0, 100), glm::vec2(80, 160));
	//}


	//update the onGround state
	for (int i = 0; i < TOTAL_TOUCH_EDGE; i++) m_touchEdges[i] = false;
	float half_height = m_body->GetFixtureList()->GetNext()->GetShape()->m_radius;

	auto center = m_body->GetPosition() + b2Vec2(0.0f, m_body->GetFixtureList()[0].GetShape()->m_radius);
	auto center2 = m_body->GetPosition() - b2Vec2(0.0f, half_height);
	Locator::GetPrimitiveRenderer()->DrawCircle(Utils::b2ToGlm(M2P*center2), 10, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

	for (b2ContactEdge*contactEdge = m_body->GetContactList(); contactEdge != nullptr; contactEdge = contactEdge->next) {
		
		b2Contact*contact = contactEdge->contact;
		if (contact->IsTouching()) {
			 
			auto otherBody = contact->GetFixtureA()->GetBody();
			if (otherBody == m_body) otherBody = contact->GetFixtureB()->GetBody();
			auto other = (BodyBase*)otherBody->GetUserData();
			if (other->GetSpecifier() == BOT_ROCK) {
			}


			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);

			//check if the point is below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_body->GetPosition().y - half_height*3.0f/2.0f) {

					Locator::GetPrimitiveRenderer()->DrawCircle(
						M2P*Utils::b2ToGlm(manifold.points[i]), 5.0f, 
						glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					m_touchEdges[BOTTOM] = true;
				}
				else if (manifold.points[i].y < m_body->GetPosition().y - half_height*5.0f / 4.0f) {
					if(manifold.points[i].x<m_body->GetPosition().x) m_touchEdges[BOTTOM_LEFT] = true;
					else m_touchEdges[BOTTOM_RIGHT] = true;
				}
				else if (manifold.points[i].y < m_body->GetPosition().y + half_height*5.0f / 4.0f) {
					if (manifold.points[i].x < m_body->GetPosition().x) {
						m_touchEdges[LEFT] = true;
						Locator::GetPrimitiveRenderer()->DrawCircle(
							M2P*Utils::b2ToGlm(manifold.points[i]), 5.0f,
							glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
					}
					else {
						m_touchEdges[RIGHT] = true;
						Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(manifold.points[i]), 5.0f,
							glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
					}
				}
				else if (manifold.points[i].y < m_body->GetPosition().y + half_height*3.0f / 2.0f) {
					if (manifold.points[i].x<m_body->GetPosition().x) m_touchEdges[TOP_LEFT] = true;
					else m_touchEdges[TOP_RIGHT] = true;
				}
				else {
					m_touchEdges[TOP] = true;
					continue;
				}
				break;
			}
		}

	}
	calculateNearestTouchPoint();

	signalState();

	if (m_handState == HandStates::DONE) {
		m_handState = HandStates::AT_REST;
		LeaveTouchPoint();
	}
	if (m_handState == HandStates::HOLD_OBJECT) {
		m_handState = HandStates::DONE;
		HoldTouchPoint(b2Vec2());
	}


	//limit the speed
	b2Vec2 vel = m_body->GetLinearVelocity();
	if (glm::abs(vel.x) > m_runSpeed) {
		float velX = (vel.x / glm::abs(vel.x))*m_runSpeed;
		float impulse = m_body->GetMass()*(velX - vel.x);
		//impulse /= 6.0f;
		m_body->ApplyLinearImpulse(b2Vec2(impulse, 0), m_body->GetWorldCenter(), true);
	}
	if (glm::abs(vel.y) > m_jumpSpeed) {
		float velY = (vel.y / glm::abs(vel.y))*m_jumpSpeed;
		float impulse = m_body->GetMass()*(velY - vel.y);
		//impulse /= 6.0f;
		m_body->ApplyLinearImpulse(b2Vec2(0, impulse), m_body->GetWorldCenter(), true);
	}
	if (glm::abs(vel.x) > 0.0001) {
		if (vel.x > 0) {
			m_dir.Set(1.0f, 0.0f);
		}
		else {
			m_dir.Set(-1.0f, 0.0f);
		}
	}
}

bool PlayerBody::CheckInStates(int index, ...)
{
	va_list ap;
	va_start(ap, index);
	while (index != -1) {
		if (index == m_currentActionState)
			return true;
		index = va_arg(ap, int);
	}
	va_end(ap);
	return false;
}



void PlayerBody::signalState()
{

	//update body state
	if (m_bodyState == BODY_STATE_NEXT_TO_ROCK_LHS) {
		m_bodyState = BODY_STATE_NEXT_TO_ROCK_LHS_2;
		m_pCallback->OnStateChanged(BS_NEXT_TO_ROCK_LHS);
	}
	else if (m_bodyState == BODY_STATE_NEXT_TO_ROCK_RHS) {
		m_bodyState = BODY_STATE_NEXT_TO_ROCK_RHS_2;
		m_pCallback->OnStateChanged(BS_NEXT_TO_ROCK_RHS);
	}
	else if (m_bodyState == BODY_STATE_ON_THE_FLOOR) {
		m_bodyState = BODY_STATE_ON_THE_FLOOR_2;
		m_pCallback->OnStateChanged(BS_ON_THE_FLOOR);
	}
}





float32 PlayerBody::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction)
{
	for (b2Fixture*a = m_body->GetFixtureList(); a; a = a->GetNext())
		if (fixture == a)
			return -1;

	if (fraction < m_minRayCastFraction) {//this is ground fixture
		m_minRayCastFraction = fraction;
		if (m_groundNormal != normal) {
			m_groundNormal = normal;
			m_newGroundNormal = true;
		}
	}

	return fraction;
}



void PlayerBody::updateAABBRadar()
{
	//AABB Query
	m_rockOnRadar = false;
	m_pTouchPoints.clear();
	m_nearestTouchPoint = NULL;
	m_iterator = m_terrain.first();

	if (m_pSkin->GetFlip() == FlipType::FLIP_NONE) {
		m_radarAABB.lowerBound = m_body->GetPosition() - b2Vec2(0.0f, 1.0f);
		m_radarAABB.upperBound = m_body->GetPosition() + b2Vec2(1.0f, 1.0f);
	}
	else if (m_pSkin->GetFlip() == FlipType::HORIZONTAL_FLIP) {
		m_radarAABB.lowerBound = m_body->GetPosition() - b2Vec2(1.0f, 1.0f);
		m_radarAABB.upperBound = m_body->GetPosition() + b2Vec2(0.0f, 1.0f);
	}
	Locator::GetPhysicsFactory()->GetB2World()->QueryAABB(this, m_radarAABB);

	if (!m_rockOnRadar) {
		if (m_bodyState != BODY_STATE_ON_THE_FLOOR_2)
			m_bodyState = BODY_STATE_ON_THE_FLOOR;
	}
	while (m_iterator != m_terrain.tail) {
		m_iterator = m_terrain.erase(m_iterator);
		m_iterator = m_iterator->next;
	}
	

	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
		m_radarAABB.lowerBound.x*M2P, m_radarAABB.lowerBound.y*M2P,
		(m_radarAABB.upperBound.x - m_radarAABB.lowerBound.x)*M2P,
		(m_radarAABB.upperBound.y - m_radarAABB.lowerBound.y)*M2P), 
		0,glm::vec2(0,0),glm::vec4(0.0f,1.0f,0.0f,1.0f));

	float i = 0;
	LIST_FOR(it, m_terrain) {
		Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(it->data), 5, glm::vec4(i, 0.0f, 1.0f, 1.0f));
		i += 0.25f;
	}
}

//Handle AABB Query 
bool PlayerBody::ReportFixture(b2Fixture * fixture)
{
	void*rawBody = fixture->GetBody()->GetUserData();
	if (rawBody == NULL)return true;
	BodyBase*body = (BodyBase*)(rawBody);
	if (body->GetBody() == m_body) return true;


	int what = body->GetSpecifier();
	if (what == BodyObjectType::BOT_TOUCH_POINT) {
		
		m_pTouchPoints.push_back(((TouchPoint*)body));
		b2AABB aabb = fixture->GetAABB(0);
		Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(
			aabb.lowerBound.x*M2P, aabb.lowerBound.y*M2P,
			(aabb.upperBound.x - aabb.lowerBound.x)*M2P,
			(aabb.upperBound.y - aabb.lowerBound.y)*M2P), 0);

	}
	if(what == BodyObjectType::BOT_ROCK){
		if (body->GetBody()->GetPosition().x - m_body->GetPosition().x < 0) {
			if(m_bodyState!= BODY_STATE_NEXT_TO_ROCK_LHS_2)
				m_bodyState = BODY_STATE_NEXT_TO_ROCK_LHS;
		}
		else {
			if (m_bodyState != BODY_STATE_NEXT_TO_ROCK_RHS_2)
				m_bodyState = BODY_STATE_NEXT_TO_ROCK_RHS;
		}
		m_rockOnRadar = true;
	}
	if (what == BodyObjectType::BOT_ROCK|| what == BodyObjectType::BOT_GROUND) {
		//collect vertices
		int n = ((b2PolygonShape*)fixture->GetShape())->m_count;
		b2Vec2 topLeft{ m_radarAABB.lowerBound.x, m_radarAABB.upperBound.y };
		b2Vec2 bottomRight{ m_radarAABB.upperBound.x, m_radarAABB.lowerBound.y };
		b2Vec2*vertices = ((b2PolygonShape*)fixture->GetShape())->m_vertices;
		b2Vec2 lastVertex;
		bool lastInside = false;

		for (int i = 0; i < n+1; i++) {
			b2Vec2 vertex = fixture->GetBody()->GetWorldPoint(vertices[i==n?0:i]);

			if (Utils::check_AABB_against_point(m_radarAABB, vertex)) {
				if (!lastInside&&i!=0) {
					float x = (lastVertex.x < vertex.x ? m_radarAABB.lowerBound.x : m_radarAABB.upperBound.x);
					float y = (lastVertex.y < vertex.y ? m_radarAABB.lowerBound.y : m_radarAABB.upperBound.y);
					b2Vec2 a;
					if (Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(x, m_radarAABB.lowerBound.y), b2Vec2(x, m_radarAABB.upperBound.y))) {
						a = Geometry::GetMidPointByX(x, lastVertex, vertex);
					}
					else if(Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(m_radarAABB.lowerBound.x,y), b2Vec2(m_radarAABB.upperBound.x,y))){
						a = Geometry::GetMidPointByY(y, lastVertex, vertex);
					}

					if (m_iterator != m_terrain.tail) {
						m_iterator->data = a;
						m_iterator = m_iterator->next;
					}
					else m_terrain.push_back(a);
				}
				lastVertex = vertex;
				lastInside = true;
			}
			else {
				if (lastInside) {
					//get the vertex on the line
					float x = (lastVertex.x > vertex.x ? m_radarAABB.lowerBound.x : m_radarAABB.upperBound.x);
					float y = (lastVertex.y > vertex.y ? m_radarAABB.lowerBound.y : m_radarAABB.upperBound.y);
					b2Vec2 a;
					if (Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(x, m_radarAABB.lowerBound.y), b2Vec2(x, m_radarAABB.upperBound.y))) {
						a = Geometry::GetMidPointByX(x, lastVertex, vertex);
					}
					else if (Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(m_radarAABB.lowerBound.x, y), b2Vec2(m_radarAABB.upperBound.x, y))) {
						a = Geometry::GetMidPointByY(y, lastVertex, vertex);
					}

					if (m_iterator != m_terrain.tail) {
						m_iterator->data = a;
						m_iterator = m_iterator->next;
					}
					else m_terrain.push_back(a);

					lastVertex = vertex;
					lastInside = false;
					continue;
				}
				else {
					lastInside = false;
					
					//we know that both vertices are not inside AABB
					if (Geometry::CheckSegmentIntersect(topLeft, bottomRight, lastVertex, vertex) ||
						Geometry::CheckSegmentIntersect(m_radarAABB.lowerBound, m_radarAABB.upperBound, lastVertex, vertex)) {
						//intersect with AABB. Now we gonna find those 2 points.
						b2Vec2 p[2] = { lastVertex, vertex }; int i = 0;


						if (Geometry::CheckSegmentIntersect(m_radarAABB.lowerBound, topLeft, p[0], p[1])) {
							b2Vec2 a = Geometry::GetMidPointByX(m_radarAABB.lowerBound.x, p[0], p[1]);
							if (p[0].x < p[1].x) p[0] = a;
							else p[1] = a;
							i++;
						}
						if (Geometry::CheckSegmentIntersect(bottomRight, m_radarAABB.upperBound, p[0], p[1])) {
							b2Vec2 a = Geometry::GetMidPointByX(m_radarAABB.upperBound.x, p[0], p[1]);
							if (p[0].x < p[1].x) p[1] = a;
							else p[0] = a;
							i++;
						}

						if (i < 2) {
							if (Geometry::CheckSegmentIntersect(m_radarAABB.lowerBound, bottomRight, p[0], p[1])) {
								b2Vec2 a = Geometry::GetMidPointByY(m_radarAABB.lowerBound.y, p[0], p[1]);
								if (p[0].y < p[1].y) p[0] = a;
								else p[1] = a;
								i++;
							}
						}
						if (i < 2) {
							if (Geometry::CheckSegmentIntersect(topLeft, m_radarAABB.upperBound, p[0], p[1])) {
								b2Vec2 a = Geometry::GetMidPointByY(m_radarAABB.upperBound.y, p[0], p[1]);
								if (p[0].y < p[1].y) p[1] = a;
								else p[0] = a;
							}
						}

						if (m_iterator != m_terrain.tail) {
							m_iterator->data = p[0];
							m_iterator = m_iterator->next;
						}
						else m_terrain.push_back(p[0]);


						//ignore the vertex
						lastVertex = vertex;
						vertex = p[1];
					}
					else {
						//ignore the vertex
						lastVertex = vertex;
						continue;
					}
				}
			}

			if (m_iterator != m_terrain.tail) {
				m_iterator->data = vertex;
				m_iterator = m_iterator->next;
			}else {
				m_terrain.push_back(vertex);
			}

		}
	}

	return true;
}

void PlayerBody::calculateNearestTouchPoint()
{
	b2Vec2 point = 1.0f / M2P * Utils::glmToB2(m_pSkin->GetArmBone1()->GetPos());
	float minDisSqr = std::numeric_limits<float>::max();
	m_nearestTouchPoint = NULL;
	for (auto&a : m_pTouchPoints) {
		float disSqr = (a->GetPos() - point).LengthSquared();
		if (minDisSqr > disSqr) {
			minDisSqr = disSqr;
			m_nearestTouchPoint = a;
		}
	}
}


void PlayerBody::_Stop()
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	if (m_touchEdges[BOTTOM]) m_body->SetLinearVelocity(b2Vec2(vel.x*0.85f, vel.y));
	else m_body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

void PlayerBody::_Move(bool left_right, float speed)
{
	m_runSpeed = speed;
	int runDir = (left_right ? 1 : -1);
	float impulse = runDir*m_body->GetMass()*m_runSpeed;
	m_body->ApplyLinearImpulse(b2Vec2(impulse, 0), m_body->GetWorldCenter(), true);

}
void PlayerBody::_Jump()
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	m_body->SetLinearVelocity(vel);
}

void PlayerBody::_JumpAndRun(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	vel.x = runDir*m_runSpeed;
	m_body->SetLinearVelocity(vel);

}

void PlayerBody::HoldTouchPoint(const b2Vec2& point)
{

	if (m_nearestTouchPoint != NULL&&m_joint==NULL) {
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_body, m_nearestTouchPoint->GetBody(), 
			m_body->GetWorldCenter(), m_nearestTouchPoint->GetBody()->GetWorldCenter());
		m_joint = (b2DistanceJoint*)Locator::GetPhysicsFactory()->GetB2World()->CreateJoint(&jointDef);
	}
}

void PlayerBody::LeaveTouchPoint()
{
	SDL_Log("Leave touch point");
	if (m_joint) {
		Locator::GetPhysicsFactory()->GetB2World()->DestroyJoint(m_joint);
		m_joint = NULL;
	}
}





void PlayerBody::RunRight()
{
	if (!IsOnGround()) return;
	_Move(true, RUN_SPEED);
	ChangeStateByIndex(AS_RUN_RIGHT);
	m_pSkin->SetFlip(FlipType::FLIP_NONE);
}

void PlayerBody::RunLeft()
{
	if (!IsOnGround()) return;
	_Move(false, RUN_SPEED);
	ChangeStateByIndex(AS_RUN_LEFT);
	m_pSkin->SetFlip(FlipType::HORIZONTAL_FLIP);
}

void PlayerBody::Jump()
{
	if (!IsOnGround()) return;
	_Jump();
	ChangeStateByIndex(AS_JUMP);
}


void PlayerBody::JumpLeft()
{
	if (!IsOnGround()) return;
	_JumpAndRun(false);
	ChangeStateByIndex(AS_JUMP);
	m_pSkin->SetFlip(FlipType::HORIZONTAL_FLIP);
}

void PlayerBody::JumpRight()
{
	if (!IsOnGround()) return;
	_JumpAndRun(true);
	ChangeStateByIndex(AS_JUMP);
	m_pSkin->SetFlip(FlipType::FLIP_NONE);
}

void PlayerBody::PushLeft()
{
	if (!IsOnGround()) return;
	_Move(false, WALK_SPEED);
	ChangeStateByIndex(AS_PUSH_LEFT);
	m_pSkin->SetFlip(FlipType::HORIZONTAL_FLIP);
}

void PlayerBody::PushRight()
{
	if (!IsOnGround()) return;
	_Move(true, WALK_SPEED);
	ChangeStateByIndex(AS_PUSH_RIGHT);
	m_pSkin->SetFlip(FlipType::FLIP_NONE);
}

void PlayerBody::PullLeft()
{
	_Move(false, WALK_SPEED);
	ChangeStateByIndex(AS_PULL_LEFT);
	m_pSkin->SetFlip(FlipType::FLIP_NONE);
}

void PlayerBody::PullRight()
{
	_Move(true, WALK_SPEED);
	ChangeStateByIndex(AS_PULL_RIGHT);
	m_pSkin->SetFlip(FlipType::HORIZONTAL_FLIP);
	if (m_handState != HandStates::HOLD_OBJECT)
		m_handState = HandStates::HOLD_OBJECT;

}

void PlayerBody::ClimbOnLeft()
{
	Locator::GetPhysicsFactory()->RunAction(new pe::Sequence(
		new pe::MoveTo(m_body, m_body->GetPosition() + b2Vec2(0, 1.0f), 1.0f),
		new pe::MoveTo(m_body,m_body->GetPosition()+b2Vec2(-0.5f,1.0f), 1.0f),
		new pe::Callback([]{SDL_Log("Callback babe");})
		,NULL));
}

void PlayerBody::ClimbOnRight()
{
	Locator::GetPhysicsFactory()->RunAction(new pe::Sequence(
		new pe::MoveTo(m_body, m_body->GetPosition() + b2Vec2(0, 1.0f), 1.0f),
		new pe::MoveTo(m_body, m_body->GetPosition() + b2Vec2(0.5f, 1.0f), 1.0f),
		new pe::Callback([] {SDL_Log("Callback babe"); })
		, NULL));
}

void PlayerBody::Stop()
{
	_Stop();
	ChangeStateByIndex(AS_STOP);
}

void PlayerBody::Stand()
{
	if (!IsOnGround()) return;
	ChangeStateByIndex(AS_STAND);
}
