#include "GameController.h"
#include"../misc/Assistances.h"
#include"../misc/GameAccessor.h"
#include"../GameObjects/SteeringBehaviors.h"
GameController::GameController(Sprite*spriteBase, Sprite*spriteButton,b2Vec2 pos)
	:m_pSpriteBase(spriteBase),m_pSpriteButton(spriteButton),m_pos0(pos), m_pos1(pos)
	,m_pSubject(NULL),m_boundRadius(100.0f),
	m_subjectInDirection(false),
	m_pressing(false),
	m_dir(0,0),m_delta(0.0f)
{
	SDL_Log("%f %f", spriteBase->GetWidth(), spriteBase->GetHeight());
}

GameController::~GameController()
{

	m_pSubject = NULL;
}

void GameController::Draw()
{
	m_pSpriteBase->Draw(0, 0, m_pos1.x, m_pos1.y,
		m_pSpriteBase->GetWidth() + m_delta*10.0f, m_pSpriteBase->GetHeight() + m_delta*10.0f,0,0.8f+m_delta*0.2f);
	m_pSpriteButton->Draw(0, 0, m_pos1.x + m_diff.x*1.5f, m_pos1.y + m_diff.y*1.5f,
		m_pSpriteButton->GetWidth() + m_delta*100.0f, m_pSpriteButton->GetHeight() + m_delta*100.0f,0,0.8f+m_delta*0.2f);
}

void GameController::Update(float deltaTime)
{
	if (m_pressing) {
		if (m_delta<1.0f)
			m_delta += deltaTime*5.0f;
	}
	else {
		m_delta = 0.0f;
	}
}

void GameController::HandleEvent(const Key & key)
{
	if (m_pSubject == NULL)
		return;

}

void GameController::HandleEvent(const Touch & touch)
{
	if (m_pSubject == NULL)
		return;

	if (touch.touching) {
		m_pressing = true;
		b2Vec2 touch0(touch.x, touch.y);
		b2Vec2 touch1(touch.cur_x, touch.cur_y);

		touch0 = s_gameAccessor.ConvertScreenToWorld(touch0);
		touch1 = s_gameAccessor.ConvertScreenToWorld(touch1);
		m_pos1 = touch0;
		m_diff = touch1 - touch0;
		if (m_diff.LengthSquared() < 0.01f)
			return;

		if (m_dir.x == 0.0f && m_dir.y == 0.0f) {
			if (m_diff.LengthSquared() > 0.1f)
				m_dir = m_diff;
			return;
		}

		m_dir = m_diff;
		m_dir.Normalize();

		Truncate(m_diff, m_boundRadius);
		m_pSubject->SetHeading(m_diff);
		if ((m_pSubject->GetHeading() - m_dir).LengthSquared() < 0.2f) {
			m_pSubject->GetSteeringBehaviors()->SetTarget(m_pSubject->GetPos() + m_diff);
			m_subjectInDirection = true;
		}
	}
	else {
		m_pos1 = m_pos0;
		m_pressing = false;
		m_diff.SetZero();
		if (m_subjectInDirection) {
			//SDL_Log("Up %f %f", m_diff.x, m_diff.y);
			m_pSubject->GetSteeringBehaviors()->SetTarget(m_pSubject->GetPos()+0.5f*m_diff);
			m_subjectInDirection = false;
		}
	}

}

void GameController::SetSubject(MovingObject * subject)
{
	m_pSubject = subject;
	m_pSubject->GetSteeringBehaviors()->ArriveOn();
}
