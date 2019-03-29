#include "Entity.h"
#include"../../misc/Locator.h"

bool AABBEntity::s_drawAABBTrigger = false;

void AABBEntity::Update(float deltaTime)
{
	if (m_selected) {
		if (!m_trigger) {
			OnSelect(m_AABB);
			m_trigger = true;
		}

		ApplyAABB(m_AABB);
	}
	else {
		if (m_trigger) {
			OnRelease(m_AABB);
			m_trigger = false;
		}

		m_AABB=CalculateAABB();
	}
}

void AABBEntity::Draw()
{
	if (s_drawAABBTrigger) {
		//visualize the AABB here...
		if (m_selected)
			Locator::GetPrimitiveRenderer()->DrawBox(
				glm::vec4(m_AABB.x, m_AABB.y, m_AABB.z - m_AABB.x, m_AABB.w - m_AABB.y), 
				0, glm::vec2(0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		else
			Locator::GetPrimitiveRenderer()->DrawBox(
				glm::vec4(m_AABB.x, m_AABB.y, m_AABB.z - m_AABB.x, m_AABB.w - m_AABB.y));
	}
}
