#include "Entity.h"
#include"../../misc/Locator.h"

bool AABBEntity::m_drawTrigger = false;

void AABBEntity::Update(float deltaTime)
{
	if (m_selected) {
		if (!m_trigger) {
			OnSelect(m_AA, m_BB);
			m_trigger = true;
		}
	}
	else {
		if (m_trigger) {
			OnRelease(m_AA, m_BB);
			m_trigger = false;
		}
	}
}

void AABBEntity::Draw()
{
	if (m_drawTrigger) {
		//visualize the AABB here...
		if (m_selected)
			Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(m_AA, m_BB - m_AA), 0, glm::vec2(0.0f), 
				glm::vec4(1.0f,0.0f,0.0f,1.0f));
		else
			Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(m_AA, m_BB - m_AA));
	}
}

bool AABBEntity::CheckAABBAgainstPoint(const glm::vec2 & point)
{
	if (point.x < m_AA.x) return false;
	if (point.x > m_BB.x) return false;
	if (point.y < m_AA.y) return false;
	if (point.y > m_BB.y) return false;
	return true;
}

bool AABBEntity::CheckAABBAgainstAABB(const glm::vec2 & aa2, const glm::vec2 & bb2)
{
	if (m_AA.x > bb2.x)return false;
	if (m_BB.x < aa2.x)return false;
	if (m_AA.y > bb2.y)return false;
	if (m_BB.y < aa2.y)return false;
	return true;
}
