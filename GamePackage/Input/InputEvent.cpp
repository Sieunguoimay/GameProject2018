#include"InputEvent.h"
#include"SDL2\SDL.h"
InputEvent::InputEvent()
{
	//for (int i = 0; i < NUM_KEYS; i++)
	//	keyPressed[i] = false;
}

void InputEvent::ProcessInputEvent(const InputEventPackage & inputEvent, 
	const glm::mat3& screenToCamera,
	const glm::vec2& cameraPos)

{
	
	if (inputEvent.type == KEY_DOWN)
		keyPressed[inputEvent.data.key] = true;
	else if (inputEvent.type == KEY_UP)
		keyPressed[inputEvent.data.key] = false;
	



	//for the problem of mouse go out of window
	glm::vec2 mousePos(inputEvent.data.motion.x, inputEvent.data.motion.y);

	static int oldX = inputEvent.data.motion.x;
	if (oldX < 100 && inputEvent.data.motion.x > 4000) mousePos.x = oldX;//4k is good 
	oldX = inputEvent.data.motion.x;


	//left mouse processing
	if (inputEvent.type == MOUSE_MOTION 
		|| inputEvent.type == LMOUSE_BUTTON_DOWN 
		|| inputEvent.type == LMOUSE_BUTTON_UP) {
		//how to send mouse pos to world

		m_leftMouse.motion = screenToCamera*glm::vec3(mousePos.x, mousePos.y,1.0f);
		m_leftMouse.mousePosInWorld= cameraPos+ m_leftMouse.motion;
	}
	if (inputEvent.type == LMOUSE_BUTTON_DOWN) {
		m_leftMouse.mousePressed = true;
		if (m_leftMouse.firstPressedPosInWorld.x == -1)
			m_leftMouse.firstPressedPosInWorld = m_leftMouse.mousePosInWorld;
		m_leftMouse.clickSignal = true;
	}
	if (inputEvent.type == LMOUSE_BUTTON_UP) {
		m_leftMouse.mousePressed = false;
		m_leftMouse.firstPressedPosInWorld = glm::vec2(-1, -1);
		m_leftMouse.releaseSignal = true;
	}

	//right mouse processing
	if (inputEvent.type == MOUSE_MOTION
		|| inputEvent.type == RMOUSE_BUTTON_DOWN
		|| inputEvent.type == RMOUSE_BUTTON_UP) {
		//how to send mouse pos to world
		m_rightMouse.motion = screenToCamera*glm::vec3(mousePos.x, mousePos.y, 1.0f);
		m_rightMouse.mousePosInWorld = cameraPos + m_rightMouse.motion;
	}
	if (inputEvent.type == RMOUSE_BUTTON_DOWN) {
		m_rightMouse.mousePressed = true;
		if (m_rightMouse.firstPressedPosInWorld.x == -1)
			m_rightMouse.firstPressedPosInWorld = m_rightMouse.mousePosInWorld;
		m_rightMouse.clickSignal = true;
	}
	if (inputEvent.type == RMOUSE_BUTTON_UP) {
		m_rightMouse.mousePressed = false;
		m_rightMouse.firstPressedPosInWorld = glm::vec2(-1, -1);
		m_rightMouse.releaseSignal = true;
	}
}

void InputEvent::ClearEvent()
{
	m_rightMouse.clickSignal = false;
	m_rightMouse.releaseSignal = false;

	m_leftMouse.clickSignal = false;
	m_leftMouse.releaseSignal = false;
}
