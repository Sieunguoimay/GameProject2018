#pragma once
#include"../../../Input/InputHandler.h"
class PlayerCommand :public Command {
	bool*trigger;
public:
	PlayerCommand(bool*trigger)
		:trigger(trigger){}
	inline void Execute()override {
		*trigger = true;
	}
	inline void ExecuteOnce()override {
		*trigger = true;
	}
};
#include"../../ObjectPool.h"
class PlayerControl :public InputHandler{
public:
	PlayerControl() { Init(); }

	void Init() override{
		AddCommand(new PlayerCommand(&m_left), HandleType::HT_KEY_A);
		AddCommand(new PlayerCommand(&m_right), HandleType::HT_KEY_D);
		AddCommand(new PlayerCommand(&m_jump), HandleType::HT_KEY_W);
		AddCommand(new PlayerCommand(&m_holdTouchPointNow), HandleType::HT_SPACE);
	}
	void Update(float deltaTime)override {
		m_jump = false;
		m_right = false;
		m_left = false;
		m_holdTouchPointNow = false;

		Command*command = handleInput();
		if (command) command->Execute();
	}

	bool m_jump;
	bool m_right;
	bool m_left;
	bool m_holdTouchPointNow;
};