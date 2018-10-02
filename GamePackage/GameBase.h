#pragma once
#include"Box2D\Box2D.h"
#include"SDL2\SDL_mixer.h"

#include"Graphics\Shaders.h"
#include"Graphics\Model3D.h"
#include"Graphics\Texture.h"
#include"Graphics\Mesh.h"
#include"Graphics\Model3D.h"
#include"Graphics\Camera.h"
#include"Controls\InputHandler.h"
#include"Controls\InputHolder.h"
#include"GameObjects\BaseObject.h"
#include"GameObjects\MovingObject.h"
#include"Controls\GameController.h"
#include"GameWorld\GameWorld.h"
#define DEFAULT_FPS 40.0f
class GameAccessor;
class GameBase
{
	//handler of this class for access from any where
	friend class GameAccessor;

	glm::mat4 m_proj;

	int m_windowWidth;
	int m_windowHeight;
	int m_gameWidth;
	int m_gameHeight;
	float m_pixelToOpenGL;//(x,y)->(ratio,1.0);

	Camera*m_mainCamera;
	InputHolder*m_inputHolder;
	GameController*m_controller;
	SteeringBehaviors*m_steering;
	std::vector<BaseObject*>m_objects;
	std::vector<MovingObject*>m_flock;
	std::vector<Sprite*>sprites;
	//Sprite*m_pBackground;
	GameWorld*m_gameWorld;
	Mix_Music*m_music;
	NoPhysicsObject*m_background;

	//Utilities

	Uint32 m_frameStart;
	int m_frameTime;
	float m_FPS;
	int m_frameDelay;

	void DrawPrimitive(GLint numVertices, GLfloat*vtxBuf, 
		GLint vtxStride, GLint numIndices, GLushort*indices);
	void SetUpOpenGL();
protected:
	bool m_done;
public:
	GameBase();
	virtual ~GameBase();

	virtual int Init(int width, int height);
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void CleanUp();
	bool Done() { return m_done; }
};
class SmallTester : public GameBase {
public:

	virtual int Init(int width, int height);
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void CleanUp();
};