#pragma once
#include"../GameObjects/EntityHierachy/PointerHolder.h"
#include"../GameObjects/EntityHierachy/Entity.h"

class Command :public PointerHolder{
public:
	//this is also a magic, not wasting a variable, and the constructor is unchanged
	//the price is that, one more function is added, though it's inline :)))
	inline virtual bool IsUnderInputHandler() { return true; }
	//->what do you mean by this??? -> whether this command is under inputhandler or other handler ???



	virtual void Execute() = 0;
	virtual void ExecuteOnce() {}
	bool m_executeOnceFlag = false;
};


//this handler can handle anything
//and it's beautiful enough for reused in the future
class InputHandler :public Entity{
public:
	enum HandleType{
		HT_KEY_A,
		HT_KEY_D,
		HT_KEY_W,
		HT_CTRL,
		HT_CTRL_A,
		HT_CTRL_S,
		HT_CTRL_D,
		HT_SHIFT,
		HT_SHIFT_D,
		HT_SPACE,
		HANDLE_TYPE_NUM
	};
	virtual ~InputHandler();
	virtual void Update(float deltaTime)override;
	void AddCommand(Command*newCommand, const HandleType&handleType);

	void executeOnce(int ht_key,int key);
protected:
	Command*handleInput();
	Command*m_commands[HANDLE_TYPE_NUM] = { 0 };
};









inline void InputHandler::Update(float deltaTime) 
{
	Command*command = handleInput();
	if (command) command->Execute();
}

inline void InputHandler::AddCommand(Command*newCommand, const HandleType&handleType)
{
	m_commands[handleType] =
		(Command*)newCommand->AddPointer((void**)&m_commands[handleType]);
}
