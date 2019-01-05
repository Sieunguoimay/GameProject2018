#include"../GameBase.h"
#include<iostream>
class GameAccessor{
	GameAccessor(){}
	GameAccessor&operator=(const GameAccessor&);
	GameAccessor(const GameAccessor&);
	GameBase*m_pGameBase;

	static GameAccessor* s_instance;
public:
	~GameAccessor() { m_pGameBase = NULL; }
	void Init(GameBase*gameBase);
	static GameAccessor&Instance();
	void CleanUp();
	//Getters
	int GetWidth() const { return this->m_pGameBase->m_windowSize.GetGameSize().w; }
	int GetHeight() const { return this->m_pGameBase->m_windowSize.GetGameSize().h; }
	int GetWindowWidth() const { return this->m_pGameBase->m_windowSize.GetWindowSize().w; }
	int GetWindowHeight() const { return this->m_pGameBase->m_windowSize.GetWindowSize().h; }
};

#define s_gameAccessor GameAccessor::Instance()