#include "Controller.h"
#define TOUCH_DIFF_LOWER_BOUND 100
void ControlEvent::Update(InputEvent * input)
{
	static bool mouse_release_trigger = false;
	
	//get control signals

	//keyboard
	m_right = input->IsKeyPressed(KEY_D);
	m_left = input->IsKeyPressed(KEY_A);
	m_up = input->IsKeyPressed(KEY_W);
	m_down = input->IsKeyPressed(KEY_S);
	m_enter = input->IsKeyPressed(KEY_ENTER);

	//mouse | touch
	if (input->IsMousePressed()) {
		m_touchDir = input->GetMousePosInWorld() - input->GetFirstPressedPosInWorld();
	
		if (m_touchDir.x > TOUCH_DIFF_LOWER_BOUND) 
			m_right = true;
		
		else if(m_touchDir.x < -TOUCH_DIFF_LOWER_BOUND)
			m_left = true;
		
		mouse_release_trigger = true;
	}
	else {
		if (mouse_release_trigger) {
			mouse_release_trigger = false;

			if (m_touchDir.y > TOUCH_DIFF_LOWER_BOUND)
				m_up = true;
		}
	}

}
