#pragma once
#include<glm\glm.hpp>
enum EventType {
	//key types
	NULL_TYPE,
	KEY_DOWN,
	KEY_UP,

	//mouse type
	MOUSE_BUTTON_DOWN,
	MOUSE_BUTTON_UP,
	MOUSE_MOTION
};
enum Key {
	NULL_KEY,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_W,
	KEY_Z,
	KEY_SPACE,
	KEY_ENTER,
	KEY_ESCAPE,
	KEY_CTRL,
	KEY_ARROW_UP,
	KEY_ARROW_DOWN,
	KEY_ARROW_LEFT,
	KEY_ARROW_RIGHT,
	NUM_KEYS
};
struct InputEventDeliver {
	InputEventDeliver(EventType type, int arg1, int arg2) 
		:type(type)
	{
		if (type == KEY_UP || type == KEY_DOWN)
			data.key = (Key)arg1;
		else {
			data.motion.x = arg1;
			data.motion.y = arg2;
		}
	}
	EventType type;
	union EventData{
		Key key;
		struct Motion {
			int x, y;
		}motion;
	}data;
};

class InputEvent {
	bool keyPressed[NUM_KEYS];
	bool mousePressed;
	glm::vec2 motion;
	glm::vec2 mousePosInWorld;
	glm::vec2 firstPressedPosInWorld;

public:
	InputEvent();

	void ProcessInputEvent(const InputEventDeliver&inputEvent, 
		const glm::mat3& screenToCamera,
		const glm::vec2& cameraPos);

	const bool IsKeyPressed(Key key) const { return keyPressed[key]; }
	const bool IsMousePressed() { return mousePressed; }
	glm::vec2 GetMousePos() { return motion; }
	glm::vec2 GetMousePosInWorld() { return mousePosInWorld; }
	glm::vec2 GetFirstPressedPosInWorld() { return firstPressedPosInWorld; }
};
