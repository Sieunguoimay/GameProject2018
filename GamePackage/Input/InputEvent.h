#pragma once
#include<glm\glm.hpp>
enum EventType {
	//key types
	NULL_TYPE,
	KEY_DOWN,
	KEY_UP,

	//mouse type
	LMOUSE_BUTTON_DOWN,
	LMOUSE_BUTTON_UP,

	RMOUSE_BUTTON_DOWN,
	RMOUSE_BUTTON_UP,

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
	KEY_SHIFT,
	KEY_ENTER,
	KEY_ESCAPE,
	KEY_CTRL,
	KEY_ARROW_UP,
	KEY_ARROW_DOWN,
	KEY_ARROW_LEFT,
	KEY_ARROW_RIGHT,
	NUM_KEYS
};
struct InputEventPackage {
	InputEventPackage(EventType type, int arg1, int arg2) 
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

class MouseEvent {
	friend class InputEvent;
	bool mousePressed;
	glm::vec2 motion;
	glm::vec2 mousePosInWorld;
	glm::vec2 firstPressedPosInWorld;

	//one time events
	bool clickSignal = false;
	bool releaseSignal = false;
public:
	MouseEvent() :mousePressed(false), firstPressedPosInWorld(-1, -1),motion(0,0){}
	inline bool& IsMousePressed() { return mousePressed; }
	inline glm::vec2& GetMousePos() { return motion; }
	inline glm::vec2& GetMousePosInWorld() { return mousePosInWorld; }
	inline glm::vec2& GetFirstPressedPosInWorld() { return firstPressedPosInWorld; }

	inline bool& IsClickSignalled() { return clickSignal; }
	inline bool& IsReleaseSignalled() { return releaseSignal; }
};



class InputEvent {
	bool keyPressed[NUM_KEYS] = { false };
	MouseEvent m_leftMouse;
	MouseEvent m_rightMouse;
public:
	InputEvent();

	void ProcessInputEvent(const InputEventPackage&inputEvent, 
		const glm::mat3& screenToCamera,
		const glm::vec2& cameraPos);
	void ClearEvent();

	const bool IsKeyPressed(int key) const { return keyPressed[key]; }

	inline MouseEvent&GetLeftMouse() { return m_leftMouse; }
	inline MouseEvent&GetRightMouse() { return m_rightMouse; }
};
