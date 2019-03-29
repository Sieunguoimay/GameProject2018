#pragma once
#include"../GameObjects/EntityHierachy/Entity.h"
#include"../misc/Locator.h"
class MouseCursor :public Entity {
	float m_cursorSize;
	glm::vec2 m_pos;
public:
	void Update(float deltaTime)override {
		InputEvent*inputEvent = Locator::GetInputEvent();
		m_pos = inputEvent->GetLeftMouse().GetMousePosInWorld();
		if (inputEvent->GetLeftMouse().IsMousePressed()
			|| inputEvent->GetRightMouse().IsMousePressed())
			m_cursorSize = 4;
		else
			m_cursorSize = 10;
	}
	void Draw()override {
		Locator::GetPrimitiveRenderer()->DrawCircle(
			m_pos - glm::vec2(m_cursorSize/2.0f, m_cursorSize / 2.0f),
			m_cursorSize, glm::vec4(1.0, 0.0, 1.0, 1.0));
	}
};
