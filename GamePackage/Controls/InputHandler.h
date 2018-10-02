#pragma once
#include"InputDispatcher.h"
struct Event {
	int type;
	int arg1, arg2;
};
enum EventType {
	DEFAULT,
};

class InputHandler {
	InputHandler*m_pNextHandler;
public:
	InputHandler() :m_pNextHandler(0) { s_inputDispatcher.RegisterHandler(this); }
	~InputHandler() { m_pNextHandler = 0; }
	virtual void HandleEvent(Event&e) = 0;

	//getters and setters
	InputHandler*GetNextHandler() { return this->m_pNextHandler; }
	void SetNextHandler(InputHandler*handler) { this->m_pNextHandler = handler; }


protected:
	void SendEvent(int type, int arg1, int arg2) { s_inputDispatcher.SendEvent(type, arg1, arg2); }
};
