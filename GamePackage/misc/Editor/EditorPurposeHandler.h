#include"../../Input/InputHandler.h"
#include"EditorCommand.h"
//this is a specification of InputHandler for the conveinient and 
//portable purpose, one time usage, high dependent, and decouple with Editor

class EditorPurposeHandler :public InputHandler {
	Editor*m_pEditor;
public:
	EditorPurposeHandler(Editor*editor)
		:m_pEditor(editor){}

	void Init() {
		AddCommand(new EditorCommand(0, m_pEditor), InputHandler::HT_CTRL_S);
		AddCommand(new EditorCommand(1, m_pEditor), InputHandler::HT_CTRL_A);
		AddCommand(new EditorCommand(2, m_pEditor), InputHandler::HT_CTRL_D);
		AddCommand(new EditorCommand(3, m_pEditor), InputHandler::HT_KEY_A);
		AddCommand(new EditorCommand(4, m_pEditor), InputHandler::HT_SHIFT);
		AddCommand(new EditorCommand(5, m_pEditor), InputHandler::HT_SHIFT_D);
	}
};