#pragma once
#include"Box2D\Box2D.h"
#include"SDL2\SDL_mixer.h"

#include<Box2D\Box2D.h>

#include"2D\Shaders.h"
#include"2D\Renderer.h"
#include"2D\PrimitiveRenderer.h"
#include"2D\Camera2D.h"
#include"2D\AnimationCollection.h"

#include"misc\WindowSizeProblem.h"
#include"misc\Timer.h"
#include"Input\InputManager.h"
#include"GameObjects\Box.h"
#include"GameObjects\Entity.h"
#include"AssetsManager.h"
#include"GameObjects\SpriterEngine\ScmlObject.h"


#define DEFAULT_FPS 40.0f
class GameAccessor;
class GameBase
{
protected:
	b2World*m_world;

	std::vector<Entity*>m_entities;

	//Utilities
	Timer m_timer;
	Box2DRenderer m_box2DRenderer;
	std::vector<Renderer*>m_renderers;
	WindowSizeProblem m_windowSize;
	Camera2D m_camera2D;

	InputManager m_inputManager;
	AssetsManager m_assetsManager;

	void SetupOpenGL(float width, float height);
	bool m_done;
public:
	GameBase();
	virtual ~GameBase();

	virtual int Init(int width, int height);
	virtual void HandleEvent(const InputEvent&inputEvent);
	virtual void Update();
	virtual void Draw();
	virtual void CleanUp();
	bool Done() { return m_done; }

	//handler of this class for access from any where
	friend class GameAccessor;
};

class SmallTester : public GameBase {

public:
	SmallTester();
	virtual int Init(int width, int height);
	virtual void Update();
	virtual void Draw();
	virtual void CleanUp();
};