#pragma once
#include"../../Input/InputHandler.h"
#include"Editor.h"
class EditorCommand :public Command {
	char m_commandIndex; //2^8=256 
	Editor*m_pEditor;
public:
	EditorCommand(char commandIndex, Editor*pEditor)
		:m_commandIndex(commandIndex), m_pEditor(pEditor)
	{}

	inline void Execute()override {
		m_pEditor->ExecuteCommand(m_commandIndex);
	}
};
