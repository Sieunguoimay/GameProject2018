#include"InputDispatcher.h"
#include"InputHandler.h"
InputDispatcher*InputDispatcher::s_instance = 0;
InputDispatcher&InputDispatcher::Instance() {
	if (s_instance == 0) {
		s_instance = new InputDispatcher();
	}
	return *s_instance;
}

void InputDispatcher::CleanUp(){
	m_pFirstHandler = 0;
	delete s_instance;
}

void InputDispatcher::SendEvent(int type, int arg1, int arg2) {
	Event e;
	e.type = type;
	e.arg1 = arg1;
	e.arg2 = arg2;
	for (InputHandler*handler = m_pFirstHandler; handler!=0; handler = handler->GetNextHandler()) {
		handler->HandleEvent(e);
	}
}
void InputDispatcher::RegisterHandler(InputHandler*newHandler) {
	newHandler->SetNextHandler(m_pFirstHandler);
	m_pFirstHandler = newHandler;
}
