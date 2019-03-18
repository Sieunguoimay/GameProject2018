#include"InputEvent.h"
InputEvent::InputEvent()
	:mousePressed(false)
	,firstPressedPosInWorld(-1, -1)
{
	for (int i = 0; i < NUM_KEYS; i++)
		keyPressed[i] = false;
	motion.x = 0;
	motion.y = 0;
}

void InputEvent::ProcessInputEvent(const InputEventDeliver & inputEvent, 
	const glm::mat3& screenToCamera,
	const glm::vec2& cameraPos)

{
	if (inputEvent.type == KEY_DOWN) {
		keyPressed[inputEvent.data.key] = true;
	}
	if (inputEvent.type == KEY_UP) {
		keyPressed[inputEvent.data.key] = false;
	}
	if (inputEvent.type == MOUSE_MOTION || inputEvent.type == MOUSE_BUTTON_DOWN || inputEvent.type == MOUSE_BUTTON_UP) {
		//how to send mouse pos to world
		motion = screenToCamera*glm::vec3(inputEvent.data.motion.x, inputEvent.data.motion.y,1.0f);
		mousePosInWorld= cameraPos+motion;
	}
	if (inputEvent.type == MOUSE_BUTTON_DOWN) {
		mousePressed = true; 
		if (firstPressedPosInWorld.x == -1) 
			firstPressedPosInWorld = mousePosInWorld;
	}
	if (inputEvent.type == MOUSE_BUTTON_UP) {
		mousePressed = false; 
		firstPressedPosInWorld = glm::vec2(-1, -1);
	}
}
