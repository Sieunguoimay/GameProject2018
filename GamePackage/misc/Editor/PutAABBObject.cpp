#include "PutAABBObject.h"
#include"../Locator.h"
void PutAABBObject::Update(float deltaTime)
{
	if (m_gotId != GWOID_OBJECT_NUM) {
		if (Locator::GetInputEvent()->GetLeftMouse().IsClickSignalled()) {
			glm::vec2 pos=Locator::GetInputEvent()->GetLeftMouse().GetMousePosInWorld();
			std::string s = std::to_string(pos.x) + " " + std::to_string(pos.y)+" "+ std::to_string(pos.x+600) + " " + std::to_string(pos.y+400);
			m_pObjectPool->CreateNewObject(new InfoPacket(m_gotId, (void*)&s),true);
		}
	}
	m_gotId = GWOID_OBJECT_NUM;
}
