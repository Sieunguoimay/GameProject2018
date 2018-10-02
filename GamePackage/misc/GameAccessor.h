#include"../GameBase.h"
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
	int GetWidth() const { return this->m_pGameBase->m_gameWidth; }
	int GetHeight() const { return this->m_pGameBase->m_gameHeight; }
	int GetWindowWidth() const { return this->m_pGameBase->m_windowWidth; }
	int GetWindowHeight() const { return this->m_pGameBase->m_windowHeight; }
	Camera*GetMainCamera() { return this->m_pGameBase->m_mainCamera; }
	glm::mat4&GetProjMatrix() { return this->m_pGameBase->m_proj; }
	float PixelToOpenGL() { return this->m_pGameBase->m_pixelToOpenGL; }
	InputHolder*GetInput() { return this->m_pGameBase->m_inputHolder; }
	GameWorld*GetGameWorld() const { return this->m_pGameBase->m_gameWorld; }

	b2Vec2 ConvertScreenToWorld(const b2Vec2&ps);
	float GetFPS() { return m_pGameBase->m_FPS; }
};

#define s_gameAccessor GameAccessor::Instance()