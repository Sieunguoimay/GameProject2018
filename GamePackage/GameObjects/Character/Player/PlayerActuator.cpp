#include "PlayerActuator.h"
#include"../../../misc/Assistances.h"
#include"../../../misc/Locator.h"
#include"../../../misc/Math/Geometry.h"
#include"../../EntityHierachy/HavingBodyEntity.h"
#include"BrainStates.h"
#include"Player.h"

PlayerActuator::PlayerActuator(Player*pOwner)
	: m_pOwner(pOwner)
	, m_jumpSpeed(3.50f)
	, m_runSpeed(RUN_SPEED)
	, m_currentActionState(AS_STAND)
	, m_handState(HandStates::AT_REST)
{
	int runIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("run");
	int climbIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("climb");
	int preClimbIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("pre_climb");
	int climbDownIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("climb_down");
	int jumpIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("jump");
	int fallIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("fall");
	int standIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("stand");
	int stopIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("stop");
	int jump2Index = m_pOwner->GetSpriterEntity()->GetAnimationIndex("jump2");
	int pullIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("walk_backward");
	int pushIndex = m_pOwner->GetSpriterEntity()->GetAnimationIndex("push");

	(m_actionStates[AS_RUN_LEFT] = new BaseActionState(runIndex,1))->SetName("Run left");
	(m_actionStates[AS_RUN_RIGHT] = new BaseActionState(runIndex,0))->SetName("Run right");
	(m_actionStates[AS_JUMP] = new _JumpingState())->SetAnimationIndex(jumpIndex)->SetName("Jump");
	(m_actionStates[AS_FALL] = new BaseActionState(fallIndex))->SetName("Fall");
	(m_actionStates[AS_JUMP_LEFT] = new _JumpingState())->SetAnimationIndex(jumpIndex)->SetFlipFlag(1)->SetName("Jump left");
	(m_actionStates[AS_JUMP_RIGHT] = new _JumpingState())->SetAnimationIndex(jumpIndex)->SetFlipFlag(0)->SetName("Jump right");
	m_actionStates[AS_PUSH_LEFT] = new BaseActionState(pushIndex,1);
	m_actionStates[AS_PUSH_RIGHT] = new BaseActionState(pushIndex,0);
	m_actionStates[AS_PULL_LEFT] = new BaseActionState(pullIndex,0);
	m_actionStates[AS_PULL_RIGHT] = new BaseActionState(pullIndex,1);
	(m_actionStates[AS_CLIMB_ON_LEFT] = new _ClimbingState(preClimbIndex, climbIndex, 1))->SetName("PreClimb");
	(m_actionStates[AS_CLIMB_ON_RIGHT] = new _ClimbingState(preClimbIndex, climbIndex,  0))->SetName("PreClimb");
	(m_actionStates[AS_CLIMB_DOWN_LEFT] = new BaseActionState(climbDownIndex, 0))->SetName("Climb Down Left");
	(m_actionStates[AS_CLIMB_DOWN_RIGHT] = new BaseActionState(climbDownIndex, 1))->SetName("Climb Down Right");
	(m_actionStates[AS_STOP] = new _StoppingState())->SetAnimationIndex(stopIndex)->SetName("Stop");
	(m_actionStates[AS_STAND] = new BaseActionState(standIndex))->SetName("Stand");

	StateMachine<PlayerActuator>::Init(this, m_actionStates[AS_STAND], NULL, NULL);
	SDL_Log("Created PlayerBody");
}

PlayerActuator::~PlayerActuator()
{
	for (int i = 0; i < AS_TOTAL_NUM; i++) delete m_actionStates[i];
	SDL_Log("Destroyed PlayerBody");
}

void PlayerActuator::Update(float deltaTime)
{
	StateMachine<PlayerActuator>::Update(deltaTime);

	if (m_handState == HandStates::DONE) {
		m_handState = HandStates::AT_REST;
		LeaveTouchPoint();
	}
	if (m_handState == HandStates::HOLD_OBJECT)
		m_handState = HandStates::DONE;


	//limit the speed
	b2Vec2 vel = m_pOwner->GetBody()->GetLinearVelocity();
	if (glm::abs(vel.x) > m_runSpeed) {
		float velX = (vel.x / glm::abs(vel.x))*m_runSpeed;
		float impulse = m_pOwner->GetBody()->GetMass()*(velX - vel.x);
		//impulse /= 6.0f;
		m_pOwner->GetBody()->ApplyLinearImpulse(b2Vec2(impulse, 0), m_pOwner->GetBody()->GetWorldCenter(), true);
	}
	if (glm::abs(vel.y) > m_jumpSpeed) {
		float velY = (vel.y / glm::abs(vel.y))*m_jumpSpeed;
		float impulse = m_pOwner->GetBody()->GetMass()*(velY - vel.y);
		//impulse /= 6.0f;
		m_pOwner->GetBody()->ApplyLinearImpulse(b2Vec2(0, impulse), m_pOwner->GetBody()->GetWorldCenter(), true);
	}

}

void PlayerActuator::_Stop()
{
	b2Vec2 vel = m_pOwner->GetBody()->GetLinearVelocity();
	if (m_pOwner->m_sensor.IsOnGround()) m_pOwner->GetBody()->SetLinearVelocity(b2Vec2(vel.x*0.85f, vel.y));
	else m_pOwner->GetBody()->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

void PlayerActuator::_Move(bool left_right, float speed)
{
	m_runSpeed = speed;
	int runDir = (left_right ? 1 : -1);
	float impulse = runDir*m_pOwner->GetBody()->GetMass()*m_runSpeed;
	m_pOwner->GetBody()->ApplyLinearImpulse(b2Vec2(impulse, 0), m_pOwner->GetBody()->GetWorldCenter(), true);

}
void PlayerActuator::_Jump()
{
	b2Vec2 vel = m_pOwner->GetBody()->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	m_pOwner->GetBody()->SetLinearVelocity(vel);
}

void PlayerActuator::_JumpAndRun(bool left_right)
{
	int runDir = (left_right ? 1 : -1);
	b2Vec2 vel = m_pOwner->GetBody()->GetLinearVelocity();
	vel.y = m_jumpSpeed;
	vel.x = runDir*m_runSpeed;
	m_pOwner->GetBody()->SetLinearVelocity(vel);

}

void PlayerActuator::HoldTouchPoint(Percept*perceptTouchPoint)
{
	if (perceptTouchPoint != NULL&&m_joint==NULL) {
		auto body = ((TouchPoint*)perceptTouchPoint->m_pBody->GetUserData())->GetOwner()->GetBody();
		SDL_Log("Hold Touch Point");
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_pOwner->GetBody(), body,
			m_pOwner->GetBody()->GetWorldCenter(), perceptTouchPoint->m_pBody->GetWorldCenter());
		m_joint = (b2DistanceJoint*)Locator::GetPhysicsFactory()->GetB2World()->CreateJoint(&jointDef);
	}
}

void PlayerActuator::LeaveTouchPoint()
{
	if (m_joint) {
		SDL_Log("Leave touch point");
		Locator::GetPhysicsFactory()->GetB2World()->DestroyJoint(m_joint);
		m_joint = NULL;
	}
}

BaseActionState*PlayerActuator::ChangeStateByIndex(int index, bool setAnimationOnEnterFlag) {
	m_actionStates[index]->m_setAnimationOnEnterFlag = setAnimationOnEnterFlag;
	ChangeState(m_actionStates[index]); 
	m_currentActionState = index; 
	return m_actionStates[index];
}

bool PlayerActuator::CheckInStates(int index, ...)
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




void PlayerActuator::RunRight()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	_Move(true, RUN_SPEED);
	ChangeStateByIndex(AS_RUN_RIGHT);
}

void PlayerActuator::RunLeft()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	_Move(false, RUN_SPEED);
	ChangeStateByIndex(AS_RUN_LEFT);
}

void PlayerActuator::Jump()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	_Jump();
	ChangeStateByIndex(AS_JUMP);
}


void PlayerActuator::JumpLeft()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	_JumpAndRun(false);
	ChangeStateByIndex(AS_JUMP_LEFT);
}

void PlayerActuator::JumpRight()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	_JumpAndRun(true);
	ChangeStateByIndex(AS_JUMP_RIGHT);
}

void PlayerActuator::PushLeft()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	_Move(false, WALK_SPEED);
	ChangeStateByIndex(AS_PUSH_LEFT);
}

void PlayerActuator::PushRight()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	_Move(true, WALK_SPEED);
	ChangeStateByIndex(AS_PUSH_RIGHT);
}

void PlayerActuator::PullLeft(Percept*perceptTouchPoint)
{
	_Move(false, WALK_SPEED);
	ChangeStateByIndex(AS_PULL_LEFT);
	if (m_handState == HandStates::AT_REST)
		HoldTouchPoint(perceptTouchPoint);
	m_handState = HandStates::HOLD_OBJECT;
}

void PlayerActuator::PullRight(Percept*perceptTouchPoint)
{
	_Move(true, WALK_SPEED);
	ChangeStateByIndex(AS_PULL_RIGHT);
	if(m_handState == HandStates::AT_REST)
		HoldTouchPoint(perceptTouchPoint);
	m_handState = HandStates::HOLD_OBJECT;
}

void PlayerActuator::ClimbOnRock(Percept*percept)
{
	if (percept == NULL)return;
	b2Vec2 p2 = percept->m_pBody->GetPosition();
	b2Vec2 p1 = m_pOwner->GetBody()->GetPosition();
	b2Vec2 dir = p2- p1;
	dir.Normalize();

	auto action = new pa::LongActionMoveUntilTouchRock(m_pOwner);
	action->Init(RUN_SPEED*dir);

	auto state = ((BrainStateLongAction*)m_pOwner->m_decisionMaker.GetState(BS_LONG_ACTION));
	state->SetFollowState(m_pOwner->m_decisionMaker.GetState(BS_ON_THE_FLOOR));
	state->AddAction(action);
	state->AddAction(new pa::LongActionClimbOnRock(m_pOwner));
	m_pOwner->m_decisionMaker.ChangeState(state);
}

void PlayerActuator::ClimbDownRockLeft(Percept * percept)
{
	auto state = ((BrainStateLongAction*)m_pOwner->m_decisionMaker.GetState(BS_LONG_ACTION));
	state->SetFollowState(m_pOwner->m_decisionMaker.GetState(BS_NEXT_TO_ROCK_RHS));
	state->AddAction(new pa::LongActionClimbDownRock(m_pOwner, AS_CLIMB_DOWN_LEFT));
	state->AddAction(new pa::LongActionDelay(0.3f, [this](float deltaTime) {
		RaiseHands(m_pOwner->GetMemory().GetPercept(PPT_CLOSEST_TOUCH_POINT), deltaTime);
	}));
	m_pOwner->m_decisionMaker.ChangeState(state);
}
void PlayerActuator::ClimbDownRockRight(Percept * percept)
{
	auto state = ((BrainStateLongAction*)m_pOwner->m_decisionMaker.GetState(BS_LONG_ACTION));
	state->SetFollowState(m_pOwner->m_decisionMaker.GetState(BS_NEXT_TO_ROCK_LHS));
	state->AddAction(new pa::LongActionClimbDownRock(m_pOwner, AS_CLIMB_DOWN_RIGHT));
	state->AddAction(new pa::LongActionDelay(0.3f, [this](float deltaTime) {
		RaiseHands(m_pOwner->GetMemory().GetPercept(PPT_CLOSEST_TOUCH_POINT), deltaTime);
	}));
	m_pOwner->m_decisionMaker.ChangeState(state);
}

void PlayerActuator::Stop()
{
	_Stop();
	ChangeStateByIndex(AS_STOP);
}

void PlayerActuator::Stand()
{
	if (!m_pOwner->m_sensor.IsOnGround()) return;
	ChangeStateByIndex(AS_STAND);
}

void PlayerActuator::RaiseHands(Percept*percept, float deltaTime)
{
	//this will raise the boneSegment to a proper angle. At the same time,
	//the three slices that keep TimelineKey(s) would be updated too.
	//after this function stops being called upon. the boneSegment will run the steering 
	//for several more loops until the animation changes at the LongActionClimbOnRock
	if (percept != NULL) {
		//percept->m_pBody;
		Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(percept->m_pBody->GetPosition()),12,glm::vec4(1.0f,0.0f,0.0f,1.0f));

		m_pOwner->m_skin.RaiseHands(M2P*Utils::b2ToGlm(percept->m_pBody->GetPosition()),deltaTime);
	}
}











void pa::LongActionMoveUntilTouchRock::Run(float deltaTime, Percept*percept)
{
	b2Vec2 currentVel = m_pOwner->GetBody()->GetLinearVelocity();
	b2Vec2 impulse = m_pOwner->GetBody()->GetMass()*b2Vec2(m_vel.x - currentVel.x,0.0f);
	m_pOwner->GetBody()->ApplyLinearImpulse(impulse, m_pOwner->GetBody()->GetWorldCenter(), true);

	m_pOwner->GetActuator().RaiseHands(m_pOwner->GetMemory().GetPercept(PPT_CLOSEST_TOUCH_POINT), deltaTime);

	if (percept != NULL) {
		if (percept->m_type == PPT_CLOSEST_ROCK && percept->m_sensingMechanism == SM_TOUCHING) {
			m_done = true;
		}
	}
}

void pa::LongActionClimbOnRock::Enter()
{
	Percept*percept = m_pOwner->GetMemory().GetPercept(PPT_CLOSEST_ROCK);
	if (percept != NULL) {
		float angle = std::fmod(std::abs(percept->m_pBody->GetAngle()), b2_pi / 2.0f);
		if (angle < b2_pi / 8.0f || angle>b2_pi*3.0f / 8.0f) {
			auto vertices = ((b2PolygonShape*)percept->m_pBody->GetFixtureList()[0].GetShape())->m_vertices;
			auto count = ((b2PolygonShape*)percept->m_pBody->GetFixtureList()[0].GetShape())->m_count;
			b2Vec2 p1 = m_pOwner->GetBody()->GetPosition();
			b2Vec2 p2 = percept->m_pBody->GetPosition();
			b2Vec2 a, b, c, d;
			float r = m_pOwner->GetBody()->GetFixtureList()[0].GetShape()->m_radius;
			for (int i = 0; i < count; i++) {
				b2Vec2 p = percept->m_pBody->GetWorldPoint(vertices[i]);
				if (p.x > p2.x) {
					if (p.y < p2.y) {
						//first point
						a = p;
					}
					else {
						//second point
						b = p;
					}
				}
				else {
					if (p.y > p2.y) {
						//third point
						c = p;
					}
					else {
						//fourth point
						d = p;
					}
				}
			}
			if (percept->m_flag == PlayerMemory::ClosestRockFlag::BODY_STATE_NEXT_TO_ROCK_RHS) {
				Utils::Swap(a, d);
				Utils::Swap(b, c);
			}
			b2Vec2 bc = c - b;
			bc.Normalize();
			bc *= 1.5f * r;

			//experienced constant that helps solving the bouncing problem at contact
			b2Vec2 first = 1.03f*(b - a) + p1;
			b2Vec2 second = first + bc;


			int state =
				m_pOwner->GetSpriterEntity()->GetFlip() == HORIZONTAL_FLIP ?
				AS_CLIMB_ON_LEFT : AS_CLIMB_ON_RIGHT;

			m_pOwner->GetSpriterEntity()->SetAnimation(
				m_pOwner->GetActuator().ChangeStateByIndex(state, false)->GetAnimationIndex(),
				Utils::LocalPointInSpartialInfoSpace(m_pOwner->GetSpriterEntity()->GetSpatialInfo(), M2P*Utils::b2ToGlm(b)));

			//Here we terminate the capturing control of the boneSegments.
			//m_pOwner->GetSkin().GetArmBone1()->ResetToIdle();
			//m_pOwner->GetSkin().GetArmBone2()->ResetToIdle();

			m_pOwner->GetSpriterEntity()->SetPos(M2P*Utils::b2ToGlm(b));
			m_pOwner->GetSpriterEntity()->SetUpdatePositionFlag(false);

			auto sequence = new pe::Sequence();
			sequence->AddAction(new pe::MoveTo(m_pOwner->GetBody(), first, 1.0f));
			sequence->AddAction(new pe::MoveTo(m_pOwner->GetBody(), second, 0.9f));
			sequence->AddAction(new pe::Callback([this]() {m_done = true; }));
			Locator::GetPhysicsFactory()->RunAction(sequence);

			return;
		}
	}
	m_done = true;
}

void pa::LongActionClimbOnRock::Exit()
{
	m_pOwner->GetSpriterEntity()->SetUpdatePositionFlag(true);
	m_pOwner->GetSpriterEntity()->SetAnimation(
	m_pOwner->GetActuator().ChangeStateByIndex(AS_STAND,false)->GetAnimationIndex(),
		Utils::LocalPointInSpartialInfoSpace(m_pOwner->GetSpriterEntity()->GetSpatialInfo(), M2P*Utils::b2ToGlm(
			m_pOwner->GetBody()->GetPosition())));
}


void pa::LongActionClimbDownRock::Enter()
{
	Percept*percept = m_pOwner->GetMemory().GetPercept(PPT_CLOSEST_ROCK);
	if (percept != NULL) {

		auto vertices = ((b2PolygonShape*)percept->m_pBody->GetFixtureList()[0].GetShape())->m_vertices;
		auto count = ((b2PolygonShape*)percept->m_pBody->GetFixtureList()[0].GetShape())->m_count;

		float r = m_pOwner->GetBody()->GetFixtureList()[0].GetShape()->m_radius;
		b2Vec2 p1 = m_pOwner->GetBody()->GetPosition();
		b2Vec2 p2 = percept->m_pBody->GetPosition();

		b2Vec2 a, b, c, d;
		for (int i = 0; i < count; i++) {
			b2Vec2 p = percept->m_pBody->GetWorldPoint(vertices[i]);
			if (p.x > p2.x) {
				if (p.y < p2.y) {
					//first point
					a = p;
				}
				else {
					//second point
					b = p;
				}
			}
			else {
				if (p.y > p2.y) {
					//third point
					c = p;
				}
				else {
					//fourth point
					d = p;
				}
			}
		}

		if (m_state == AS_CLIMB_DOWN_RIGHT) {
			Utils::Swap(a, d);
			Utils::Swap(b, c);
		}
			

		b2Vec2 bc = c - b;
		bc.Normalize();
		b2Vec2 dc = c-d;
		b2Vec2 cp1 = p1 - c;
		b2Vec2 proj = ((cp1.x*dc.x + cp1.y*dc.y) / dc.LengthSquared())*dc;

		b2Vec2 first = c+((r+0.05f)*bc+proj);
		b2Vec2 second = first - dc;
		glm::vec2 animationPivot = M2P * Utils::b2ToGlm(c);


		m_pOwner->GetSpriterEntity()->SetAnimation(
			m_pOwner->GetActuator().ChangeStateByIndex(m_state, false)->GetAnimationIndex(),
			Utils::LocalPointInSpartialInfoSpace(m_pOwner->GetSpriterEntity()->GetSpatialInfo(), animationPivot));

		m_pOwner->GetSpriterEntity()->SetPos(animationPivot);
		m_pOwner->GetSpriterEntity()->SetUpdatePositionFlag(false);

		auto sequence = new pe::Sequence();
		Locator::GetPhysicsFactory()->RunAction(&sequence
			->AddAction(new pe::MoveTo(m_pOwner->GetBody(), first, 0.5f))
			.AddAction(new pe::MoveTo(m_pOwner->GetBody(), second, 0.5f))
			.AddAction(new pe::Callback([this]() {m_done = true; })));
		return;
	}
	m_done = true;

}

void pa::LongActionClimbDownRock::Run(float deltaTime, Percept * percept)
{
	//b2Vec2 p1 = m_pOwner->GetBody()->GetPosition();
	//b2Vec2 dc = c - d;
	//b2Vec2 cp1 = p1 - c;
	//b2Vec2 proj = ((cp1.x*dc.x + cp1.y*dc.y) / dc.LengthSquared())*dc;

	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(first), 10);
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(second), 10);
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(a), 10);
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(b), 9);
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(c), 7);
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(d), 5);
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(p1), 5);
	//Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(c+proj), 5);
}

void pa::LongActionClimbDownRock::Exit()
{
	m_pOwner->GetSpriterEntity()->SetUpdatePositionFlag(true);
	m_pOwner->GetSpriterEntity()->SetAnimation(
		m_pOwner->GetActuator().ChangeStateByIndex(AS_STAND, false)->GetAnimationIndex(),
		Utils::LocalPointInSpartialInfoSpace(
			m_pOwner->GetSpriterEntity()->GetSpatialInfo(),
			M2P*Utils::b2ToGlm(m_pOwner->GetBody()->GetPosition())));
}


void pa::ActionCallback::Run(float deltaTime, Percept * percept)
{
	m_callback();
}
