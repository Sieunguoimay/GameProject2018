#include "InputHandler.h"
#include"../misc/Locator.h"
InputHandler::~InputHandler()
{
	for (int i = 0; i < HANDLE_TYPE_NUM; i++)
		if (m_commands[i]) {
			if (m_commands[i]->IsUnderInputHandler())
				delete m_commands[i];
		}
}

Command * InputHandler::handleInput()
{
	//priority matters here...
	if (m_commands[HT_CTRL_A] 
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_CTRL) 
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_A))
		return m_commands[HT_CTRL_A];

	if (m_commands[HT_CTRL_S]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_CTRL)
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_S))
		return m_commands[HT_CTRL_S];

	if (m_commands[HT_CTRL_D]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_CTRL)
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_D))
		return m_commands[HT_CTRL_D];

	if (m_commands[HT_CTRL]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_CTRL))
		return m_commands[HT_CTRL];


	if (m_commands[HT_SHIFT_D]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_SHIFT)
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_D))
		return m_commands[HT_SHIFT_D];

	if (m_commands[HT_SHIFT]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_SHIFT))
		return m_commands[HT_SHIFT];


	//control command
	if (m_commands[HT_SPACE]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_SPACE))
		m_commands[HT_SPACE]->Execute();

	if (m_commands[HT_KEY_A]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_A))
		{m_commands[HT_KEY_A]->Execute();}
	
	if (m_commands[HT_KEY_D]
		&& Locator::GetInputEvent()->IsKeyPressed(KEY_D))
		{m_commands[HT_KEY_D]->Execute();}

	executeOnce(HT_KEY_W, KEY_W);

	return NULL;
	//want more??... 
}

void InputHandler::executeOnce(int ht_key, int key)
{

	if (m_commands[ht_key]
		&& Locator::GetInputEvent()->IsKeyPressed(key))
	{
		if (!m_commands[ht_key]->m_executeOnceFlag) {
			m_commands[ht_key]->ExecuteOnce();
			m_commands[ht_key]->m_executeOnceFlag = true;
		}
	}
	else {
		m_commands[ht_key]->m_executeOnceFlag = false;
	}

}
