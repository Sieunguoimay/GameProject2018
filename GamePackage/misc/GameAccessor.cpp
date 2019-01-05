#include"GameAccessor.h"
GameAccessor*GameAccessor::s_instance = NULL;
GameAccessor&GameAccessor::Instance() {
	if (s_instance == NULL)
		s_instance = new GameAccessor();
	return *s_instance;
}
void GameAccessor::Init(GameBase*gameBase) {
	m_pGameBase = gameBase;
}
void GameAccessor::CleanUp() {
	delete s_instance;
}

