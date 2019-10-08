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

#include"Input\InputEvent.h"
#include"Input\Controller.h"

#include"AssetsManager.h"
#include"GameObjects\PhysicsEngine\PhysicsFactory.h"
#include"misc\List.h"
#define DEFAULT_FPS 40.0f
class GameBase
{
protected:
	enum ToolType {
		OBJECT_POOL,
		WORLD,
		EDITOR,
		CAMERA,
		CAMERA_DRAG,
		MOUSE_CURSOR,
		TOOL_NUM
	};

	Entity*m_tools[TOOL_NUM] = {0};
	List<AABBEntity*>m_entities;

	//Utilities
	Timer m_timer;
	Box2DRenderer m_box2DRenderer;
	std::vector<Renderer*>m_renderers;
	WindowSizeProblem m_windowSize;
	Camera2D m_camera2D;

	InputEvent m_inputEvent;
	AssetsManager m_assetsManager;
	PhysicsFactory m_physicsFactory;

	//Editor m_editor;

	void setupOpenGL(float width, float height);
	bool m_done;
public:
	GameBase();
	virtual ~GameBase();
	virtual void InitGameObjects()=0;

	virtual int Init(int width, int height);
	virtual void HandleEvent(const InputEventPackage&inputEvent);
	virtual void Update()=0;
	virtual void Draw();
	virtual void CleanUp();
	bool Done() { return m_done; }

};


