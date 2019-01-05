#include"Player.h"
#include"../../misc/Locator.h"






Capsule::Capsule(b2World * world, const glm::vec2 & pos, const glm::vec2 & dimension, float density, float friction, bool fixedRotation)
{
	m_dimension = dimension;
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x, -0.5);//Example: if x is 100 then position.x is 1.0f
	bd.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bd);



	//Create the circles
	b2CircleShape cs;
	cs.m_radius = dimension.x / 200.0f;

	b2FixtureDef cd;
	cd.shape = &cs;
	cd.density = density;
	cd.friction = friction;
	cd.restitution = 0.15f;
	cs.m_p.Set(0.0f, (-dimension.y + dimension.x) / 200.0f);
	m_fixtures[0] = m_body->CreateFixture(&cd);

	cs.m_p.Set(0.0f, (+dimension.y - dimension.x) / 200.0f);
	m_fixtures[1] = m_body->CreateFixture(&cd);

}
Capsule::~Capsule()
{
}
void Capsule::Update(float deltaTime) {
	m_position = glm::vec2(
		(m_body->GetPosition().x*100.0f),
		(m_body->GetPosition().y*100.0f));


}

void Capsule::Draw() {

	glm::vec4 destRect = glm::vec4(
		m_position.x - m_dimension.x / 2.0f,
		m_position.y - (m_dimension.y- m_dimension.x) / 2.0f,
		m_dimension.x,
		m_dimension.y - m_dimension.x);
	Locator::GetPrimitiveRenderer()->DrawBox(destRect, m_body->GetAngle(),glm::vec2(destRect.z/2,destRect.w/2));

	//draw circles
	Locator::GetPrimitiveRenderer()->DrawCircle(m_position + glm::vec2(0.0f, (-m_dimension.y + m_dimension.x) / 2.0f), m_dimension.x/2.0f);
	Locator::GetPrimitiveRenderer()->DrawCircle(m_position + glm::vec2(0.0f, (m_dimension.y - m_dimension.x) / 2.0f), m_dimension.x/2.0f);
}









Player::Player(b2World*world, const glm::vec2& pos
	, const glm::vec2& collisionDimension
	, const glm::vec2& drawDimension
	, AnimationCollection*animationCollection)
	:m_capsule(world, pos, collisionDimension, 1, 0.5, true)
	,m_animation(animationCollection), m_drawDimension(drawDimension) {

	m_states.push_back(new StandingState());
	m_states.push_back(new InTheAirState());
	m_states.push_back(new WalkingState());
	m_stateMachine.Init(this, m_states[IN_THE_AIR_STATE], NULL, NULL);
	m_runSpeed = 3.0f;
	m_jumpSpeed = 5.0f;

}

Player::~Player() {
	for (auto&state : m_states)
		delete state;
	delete m_animation;
}

void Player::Update(float deltaTime) {
	checkOnGround();
	m_stateMachine.Update(deltaTime);
	m_behavior.Update(this, deltaTime);
	m_capsule.Update(deltaTime);
	m_animation->Update(deltaTime);
}

void Player::Draw() {
	////debug draw
	//m_capsule.Draw();

	////effect draw
	//Locator::GetPrimitiveRenderer()->DrawLight(m_capsule.GetPosition(), 500,
	//	glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	glm::ivec2 dimension = m_drawDimension;
	glm::vec4 destRect = glm::vec4(m_capsule.GetPosition()- (glm::vec2)dimension/2.0f, dimension);

	//player texture
	//Locator::GetTextureRenderer()->Draw(destRect, glm::vec4(0, 0, 1, 1), m_animation.GetTextureID(), 0, color);
	static bool flip = false;
	if (m_capsule.GetBody()->GetLinearVelocity().x < -1.0f)
		flip = true;
	if (m_capsule.GetBody()->GetLinearVelocity().x > 1.0f)
		flip = false;
	Locator::GetTextureRenderer()->Draw(destRect, m_animation->GetCurrentSpriteRect(), m_animation->GetTextureID(), -1.0f, glm::vec4(1.0f),flip);
}

void Player::checkOnGround()
{
	m_onGround = false;
	b2Body*body = m_capsule.GetBody();
	float half_height = m_capsule.GetDimension().y / 400.0f;

	for (b2ContactEdge*contactEdge = body->GetContactList(); contactEdge != nullptr; contactEdge = contactEdge->next) {
		b2Contact*contact = contactEdge->contact;
		if (contact->IsTouching()) {
			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);
			//check if the point is below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - half_height ) {
					m_onGround = true;
					return;
				}
			}
		}
	}
}












void PlayerBehavior::Update(Player*player, float deltaTime)
{
	Capsule capsule = player->GetCapsule();
	b2Body*body = capsule.GetBody();
	StateMachine<Player>&stateMachine = player->GetStateMachine();

	bool controled = false;
	bool onGround = player->IsOnGround();

	bool jump = Locator::GetInput()->IsKeyPressed(KEY_W);
	bool run_left = Locator::GetInput()->IsKeyPressed(KEY_A);
	bool run_right = Locator::GetInput()->IsKeyPressed(KEY_D);

	//static float desiredAngle = -glm::radians(90.0f);

	//if (run_left) {
	//	desiredAngle += glm::radians(2.0f*360.0f*deltaTime);
	//}else if (run_right) {
	//	desiredAngle -= glm::radians(2.0f*360.0f*deltaTime);
	//}
	//desiredAngle = glm::clamp(desiredAngle, -glm::radians(120.0f), -glm::radians(60.0f));
	//joint->SetAngularOffset(desiredAngle);


	//car class function
	//float lockAngle = glm::radians(40.0f);
	//float turnSpeedPerSec = glm::radians(320.0f);//from lock to lock in 0.25 sec
	//float turnPerTimeStep = turnSpeedPerSec / 60.0f;
	//static float desiredAngle = 0;

	//if (run_left) {
	//	desiredAngle += glm::radians(5.0f);
	//}else if (run_right) {
	//	desiredAngle -= glm::radians(5.0f);
	//}
	//float angleNow = joint->GetJointAngle();
	//float angleToTurn = desiredAngle - angleNow;
	//angleToTurn = b2Clamp(angleToTurn, -turnPerTimeStep, turnPerTimeStep);
	//float newAngle = angleNow + angleToTurn;
	//joint->SetLimits(newAngle, newAngle);

	int runDir = (run_left ? -1 : (run_right?1:0));
	if (onGround) {
		if ((run_left || run_right) && jump) {
			b2Body *body = player->GetCapsule().GetBody();
			b2Vec2 vel = body->GetLinearVelocity();
			vel.y = player->GetJumpSpeed();
			vel.x = runDir*player->GetRunSpeed();
			body->SetLinearVelocity(vel);
		}
		else if (run_left || run_right) {
			float impulse = runDir*body->GetMass()*player->GetRunSpeed();
			impulse /= 6.0f;
			body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
		}
		else if (jump) {
			//float impulse = body->GetMass()*JUMP_SPEED;
			//impulse /= 3.0f;
			//body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter(), true);
			b2Body *body = player->GetCapsule().GetBody();
			b2Vec2 vel = body->GetLinearVelocity();
			vel.y = player->GetJumpSpeed();
			body->SetLinearVelocity(vel);
		}
	}
	b2Vec2 vel = body->GetLinearVelocity();
	if (!controled) {
		if (onGround) body->SetLinearVelocity(b2Vec2(vel.x*0.85, vel.y));
		else body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	}
	if (glm::abs(vel.x) > player->GetRunSpeed()) {
		float velX = (vel.x / glm::abs(vel.x))*player->GetRunSpeed();
		float impulse = body->GetMass()*(velX-vel.x);
		impulse /= 6.0f;
		body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
	}
	if (glm::abs(vel.y) > player->GetJumpSpeed()) {
		float velY = (vel.y / glm::abs(vel.y))*player->GetJumpSpeed();
		float impulse = body->GetMass()*(velY - vel.y);
		impulse /= 6.0f;
		body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter(), true);
	}
}









void StandingState::Enter(Player * player)
{
	SDL_Log("Standing");
	player->GetAnimationCollection()->SwitchAnimation("standing");
}

void StandingState::Execute(Player * player,float deltaTime)
{
	b2Vec2 vel = player->GetCapsule().GetBody()->GetLinearVelocity();
	if (player->IsOnGround()&&glm::abs(vel.x)>0.05)
		player->GetStateMachine().ChangeState(player->GetState(RUNNING_STATE));
	else if (glm::abs(vel.y)>1.0)
		player->GetStateMachine().ChangeState(player->GetState(IN_THE_AIR_STATE));

}

void StandingState::Exit(Player * player)
{
}




void InTheAirState::Enter(Player * player)
{
	SDL_Log("In the air");
	player->GetAnimationCollection()->SwitchAnimation("jumping");


}

void InTheAirState::Execute(Player * player,float deltaTime)
{
	b2Vec2 vel = player->GetCapsule().GetBody()->GetLinearVelocity();
	if (player->IsOnGround()&&glm::abs(vel.y)<0.05) {
		player->GetStateMachine().ChangeState(player->GetState(STANDING_STATE));
	}
}

void InTheAirState::Exit(Player * player)
{
}









void WalkingState::Enter(Player * player)
{
	SDL_Log("Running");
	player->GetAnimationCollection()->SwitchAnimation("running");
}

void WalkingState::Execute(Player * player,float deltaTime)
{
	b2Vec2 vel = player->GetCapsule().GetBody()->GetLinearVelocity();
	if(glm::abs(vel.x)<0.05)
		player->GetStateMachine().ChangeState(player->GetState(STANDING_STATE));
	else if (glm::abs(vel.y)>1.0)
		player->GetStateMachine().ChangeState(player->GetState(IN_THE_AIR_STATE));
}

void WalkingState::Exit(Player * player)
{
}
