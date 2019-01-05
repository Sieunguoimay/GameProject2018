#include "CharacterBrain.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"
#include"TerrestrialBody.h"

CharacterBrain::CharacterBrain(BodyBase*body, Skin*skin)
	:Brain(body,skin)
{
	m_running = new _RunningState();
	m_jumping = new _JumpingState();
	m_standing = new _StandingState();
	m_stopping = new _StoppingState();
	m_jumpingBuffer = new _JumpingBufferState();

	StateMachine<CharacterBrain>::Init(this,m_standing, NULL, NULL);

	m_pSkin->SetAnimationSwitchingTime("stand", "run", 200);
	m_pSkin->SetAnimationSwitchingTime("run", "stand", 200);
	m_pSkin->SetAnimationSwitchingTime("jump", "jump", 200);
	m_pSkin->SetAnimationSwitchingTime("stand", "jump", 180);
	m_pSkin->SetAnimationSwitchingTime("run", "jump", 180);
	m_pSkin->SetAnimationSwitchingTime("run", "stop", 200);
	m_pSkin->SetAnimationSwitchingTime("stop", "stand", 300);
	m_pSkin->SetAnimationSwitchingTime("jump", "jump2", 20);
	m_pSkin->SetAnimationSwitchingTime("jump2", "jump", 20);
}

CharacterBrain::~CharacterBrain()
{
	delete m_running;
	delete m_jumping;
	delete m_standing;
	delete m_stopping;
	delete m_jumpingBuffer;
}

void CharacterBrain::Update(float deltaTime)
{
	StateMachine<CharacterBrain>::Update(deltaTime);
	//get control signals
	bool jump = Locator::GetInput()->IsKeyPressed(KEY_W);
	bool run_left = Locator::GetInput()->IsKeyPressed(KEY_A);
	bool run_right = Locator::GetInput()->IsKeyPressed(KEY_D);
	static bool s_jump_flag = false;//to deal with the 2 loops in one jump

	if (Locator::GetInput()->IsMousePressed()) {
		int x = Locator::GetInput()->GetMousePosInWorld().x;
		if(x > m_pBody->GetBodyKit().GetPosition().x+20)
			run_right=true;
		else if(x<m_pBody->GetBodyKit().GetPosition().x-20)
			run_left = true;	
	}



	//when to do what logics
	TerrestrialBody*body = (TerrestrialBody*)(m_pBody);
	if (body->IsOnGround()) {
		if (run_left || run_right)
			m_pSkin->SetFlip(run_right ? FLIP_NONE : HORIZONTAL_FLIP);

		if ((run_left || run_right) && jump) {//run and jump

			body->JumpAndRun(run_right);//called again and again

			//called only one for differences										
			if (s_jump_flag) {//from air to land
				StateMachine<CharacterBrain>::ChangeState(m_jumpingBuffer);
			}
			else {
				StateMachine<CharacterBrain>::ChangeState(m_jumping);
			}
		}
		else if (run_left || run_right) {//run
			body->Run(run_right);
			StateMachine<CharacterBrain>::ChangeState(m_running);
		}
		else if (jump) {
			body->Jump();
			if (s_jump_flag) {//from air to land
				StateMachine<CharacterBrain>::ChangeState(m_jumpingBuffer);
			}
			else {
				StateMachine<CharacterBrain>::ChangeState(m_jumping);
			}
		}
		else {//stand
			body->Stop();
			if(StateMachine<CharacterBrain>::IsInState(m_running)|| StateMachine<CharacterBrain>::IsInState(m_stopping))
				StateMachine<CharacterBrain>::ChangeState(m_stopping);
			else
				StateMachine<CharacterBrain>::ChangeState(m_standing);
		}
		s_jump_flag = false;
	}
	else {//on air
		s_jump_flag = true;
	}
}



void _StandingState::Enter(CharacterBrain * entity)
{
	SDL_Log("Standing\n");
	entity->GetSkin()->SetAnimation("stand");
}

void _StandingState::Execute(CharacterBrain * entity, float deltaTime)
{
}

void _StandingState::Exit(CharacterBrain * entity)
{
}




void _JumpingState::Enter(CharacterBrain * entity)
{
	SDL_Log("Jumping\n");
	entity->GetSkin()->SetAnimation("jump");
}

void _JumpingState::Execute(CharacterBrain * entity, float deltaTime)
{
}

void _JumpingState::Exit(CharacterBrain * entity)
{
}




void _RunningState::Enter(CharacterBrain * entity)
{
	SDL_Log("Running\n");
	entity->GetSkin()->SetAnimation("run");
}

void _RunningState::Execute(CharacterBrain * entity, float deltaTime)
{
}

void _RunningState::Exit(CharacterBrain * entity)
{
}

void _StoppingState::Enter(CharacterBrain * entity)
{
	SDL_Log("Stopping From Running to Standing");
	entity->GetSkin()->SetAnimation("stop");
}

void _StoppingState::Execute(CharacterBrain * entity, float deltaTime)
{
	if (entity->GetSkin()->GetSpriterEntity()->HasDone()) {
		entity->ChangeState(entity->m_standing);
	}
}

void _StoppingState::Exit(CharacterBrain * entity)
{
}



void _JumpingBufferState::Enter(CharacterBrain * entity)
{
	entity->GetSkin()->SetAnimation("jump2");
	SDL_Log("HEYZO jumping buffer is here");
}

void _JumpingBufferState::Execute(CharacterBrain * entity, float deltaTime)
{
	if (entity->GetSkin()->GetSpriterEntity()->HasDone()) {
		entity->ChangeState(entity->m_jumping);
	}
}

void _JumpingBufferState::Exit(CharacterBrain * entity)
{
}
