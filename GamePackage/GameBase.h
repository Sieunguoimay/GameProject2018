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
#include"Input\Controller.h"

#include"AssetsManager.h"
#include"misc\Editor\Editor.h"
#include"GameObjects\PhysicsEngine\PhysicsFactory.h"

#define DEFAULT_FPS 40.0f
class GameBase
{
protected:
	std::vector<Entity*>m_entities;

	//Utilities
	Timer m_timer;
	Box2DRenderer m_box2DRenderer;
	std::vector<Renderer*>m_renderers;
	WindowSizeProblem m_windowSize;
	Camera2D m_camera2D;

	Controller m_controller;
	InputManager m_inputManager;
	AssetsManager m_assetsManager;
	PhysicsFactory m_physicsFactory;

	void SetupOpenGL(float width, float height);
	bool m_done;
public:
	GameBase();
	virtual ~GameBase();
	virtual void InitGameObjects();

	virtual int Init(int width, int height);
	virtual void HandleEvent(const InputEvent&inputEvent);
	virtual void Update();
	virtual void Draw();
	virtual void CleanUp();
	bool Done() { return m_done; }

};





class SmallTester : public GameBase {
protected:
public:
	SmallTester();
	void InitGameObjects()override;

	virtual int Init(int width, int height);
	virtual void Update();
	virtual void Draw();
	virtual void CleanUp();
};