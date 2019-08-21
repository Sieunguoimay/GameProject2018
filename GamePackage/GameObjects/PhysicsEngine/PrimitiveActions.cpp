#include "PrimitiveActions.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"

pe::MoveTo::MoveTo(b2Body * pBody, const b2Vec2 & target, float duration)
	:TangibleAction(pBody, duration), m_target(target)
{
	printf("MoveTo created\n");
	m_oldPos = m_pBody->GetPosition();
	m_pBody->SetGravityScale(0.0f);
}


pe::MoveTo::~MoveTo(){
	m_pBody->SetGravityScale(1.0f);
	printf("MoveTo deleted\n");
}


void pe::MoveTo::Run(float deltaTime){
	TangibleAction::Run(deltaTime);
	
	b2Vec2 diff = m_target - m_pBody->GetPosition();
	if (diff.LengthSquared() < 0.005f) {
		m_timer = m_duration;
		return;
	}

	b2Vec2 vel = (1.0f / (std::max(m_duration - m_timer, 0.01f)))*diff;
	b2Vec2 currentVel = m_pBody->GetLinearVelocity();

	b2Vec2 impulse = m_pBody->GetMass()*(vel - currentVel);
	m_pBody->ApplyLinearImpulse(impulse, m_pBody->GetWorldCenter(), true);

	Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(m_oldPos), 10);
	Locator::GetPrimitiveRenderer()->DrawCircle(M2P*Utils::b2ToGlm(m_target), 10);
}

pe::Sequence::Sequence(BaseAction * action, ...)
	:m_actionIndex(0)
{
	va_list ap;
	va_start(ap, action);
	while (action != NULL) {
		m_actions.push_back(action);
		action = va_arg(ap, BaseAction*);
	}
	va_end(ap);
}

pe::Sequence::~Sequence()
{
	for (auto&a : m_actions) delete a;
}

void pe::Sequence::Run(float deltaTime)
{
	if (m_actionIndex >= m_actions.size()) return;
	m_actions[m_actionIndex]->Run(deltaTime);
	if (m_actions[m_actionIndex]->Timeout()) m_actionIndex++;
}
