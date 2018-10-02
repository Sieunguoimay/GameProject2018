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

b2Vec2 GameAccessor::ConvertScreenToWorld(const b2Vec2 & ps)
{
	float32 w = float32(GetWindowWidth());
	float32 h = float32(GetWindowHeight());
	float32 u = ps.x / w;
	float32 v = (h - ps.y) / h;

	float32 ratio = w / h;
	b2Vec2 extents(ratio*0.5f / PixelToOpenGL(), 0.5f / PixelToOpenGL());
	//extents *= m_zoom;

	b2Vec2 lower = -extents;
	b2Vec2 upper = extents;

	b2Vec2 pw;
	pw.x = (1.0f - u) * lower.x + u * upper.x;
	pw.y = (1.0f - v) * lower.y + v * upper.y;
	return pw;
}

