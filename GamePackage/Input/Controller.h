#pragma once
#include"InputEvent.h"
class ControlEvent {
	bool m_up;
	bool m_down;
	bool m_left;
	bool m_right;
	bool m_enter;

	glm::vec2 m_touchDir;
public:
	void Update(InputEvent*input);

	inline const bool& Right() { return m_right; }
	inline const bool& Left() { return m_left; }
	inline const bool& Up() { return m_up; }
	inline const bool& Down() { return m_down; }
	inline const bool& Enter() { return m_enter; }

	//bool A();
	//bool B();
	//bool C();
};
