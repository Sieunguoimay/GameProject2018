#pragma once
class InputHandler;
class InputDispatcher {
	friend class InputHandler;
	InputDispatcher():m_pFirstHandler(0){}

	InputDispatcher&operator=(const InputDispatcher&);
	InputDispatcher(const InputDispatcher&);
	static InputDispatcher*s_instance;

	InputHandler*m_pFirstHandler;
	void RegisterHandler(InputHandler*newHandler);
public:
	void Init(){}
	static InputDispatcher&Instance();
	void CleanUp();
	~InputDispatcher(){}



	void SendEvent(int type, int arg1, int arg2);

};
#define s_inputDispatcher InputDispatcher::Instance()