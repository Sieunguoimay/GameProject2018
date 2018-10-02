#include "GameBase.h"
#include<ctime>
#include"SDL2\SDL.h"
#include"SDL2\SDL_opengles2.h"
#include"glm/glm.hpp"
#include"glm\gtc\matrix_transform.hpp"
#include"Graphics\ResourceManager.h"
#include"Graphics\SceneManager.h"
#include"misc/Assistances.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_mixer.h"
#include"Controls\InputHolder.h"
#include"2D\SpriteManager.h"
#include"misc\GameAccessor.h"
#include"Physics\PhysicsManager.h"
#include"GameObjects\SteeringBehaviors.h"
#include"SDL2\SDL_rwops.h"
GameBase::GameBase()
{
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);//for loading files, logging, Timimg, etc 
	IMG_Init(IMG_INIT_PNG);//to load png image
	Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_FLAC);//music
	SDL_Log("Created Game Base");
}


GameBase::~GameBase()
{
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_Log("Destroyted Game Base");
}
void GameBase::SetUpOpenGL()
{
	float gameSizeOrPerspectiveRatio = (float)m_gameWidth / (float)m_gameHeight;
	m_proj = glm::perspective(glm::radians(60.0f), gameSizeOrPerspectiveRatio, 0.1f, 100.0f);

	float viewportX = 0.0f;
	float viewportY = 0.0f;
	float viewportW;
	float viewportH;

	float windowRatio = (float)m_windowWidth / (float)m_windowHeight;
	if (windowRatio < gameSizeOrPerspectiveRatio) {
		viewportW = m_windowWidth;
		viewportH = m_windowWidth / gameSizeOrPerspectiveRatio;
		viewportY = (m_windowHeight - viewportH) / 2.0f;
	}else{
		viewportW = m_windowHeight * gameSizeOrPerspectiveRatio;
		viewportH = m_windowHeight;
		viewportX = (m_windowWidth -viewportW) / 2.0f;
	}
	glViewport(viewportX, viewportY, viewportW, viewportH);

	glClearColor(64.0f/255.0f, 65.0f/255.0f, 68.0f/255.0f, 1.0f);
	glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (gameSizeOrPerspectiveRatio > 1.0f) 
		m_pixelToOpenGL = 1.0f / m_gameHeight;
	else	//never happens, in case perspective can accept this ratio
		m_pixelToOpenGL = 1.0f / m_gameWidth;
}
int GameBase::Init(int width, int height)
{
	//game running
	this->m_done = false;

	//this device size is not fixed
	this->m_windowWidth = width;
	this->m_windowHeight = height;

	//this is what we care about, the game world in pixel size
	this->m_gameWidth = 1920;
	this->m_gameHeight = 1080;
	
	//timming
	this->m_FPS = DEFAULT_FPS;
	this->m_frameDelay = 1000 / m_FPS;
	this->m_frameStart = SDL_GetTicks();

	this->SetUpOpenGL();

	s_inputDispatcher.Init();
	s_gameAccessor.Init(this);
	s_resourceManager.Init("Resources/Resources.txt");
	s_sceneManager.Init("Resources/Scene.txt");
	s_spriteManager.Init("Resources/Sprites.txt");
	s_physicsManager.Init(b2Vec2(0.0f, 0.0f), 100.0f);//Box2D stays here

	std::vector<Shaders*>*shaders = s_resourceManager.GetAllShaders();
	std::vector<Light*>&lights = *s_sceneManager.GetAllLight();
	for (auto&shader : *shaders) {
		shader->UseProgram();
		glUniformMatrix4fv(shader->u_proj, 1, GL_FALSE, &(m_proj[0][0]));
		for (int i = 0; i < lights.size(); i++)
			lights[i]->SendDataToShaders(shader,i);
	}

	this->m_mainCamera = s_sceneManager.GetCurrentCamera();
	this->m_inputHolder = new InputHolder();






	this->m_gameWorld = new GameWorld();
	this->m_gameWorld->Init("Resources/Maps/map.txt");
	this->m_gameWorld->SetWallBody(s_physicsManager.CreateChainBody(m_gameWorld->GetWallVertices()));

	Sprite*temp = s_spriteManager.GetSprite("controller_base");
	Sprite*temp2 = s_spriteManager.GetSprite("controller_button");
	this->m_controller = new GameController(temp,temp2,b2Vec2(-m_gameWidth/2.0f+temp->GetWidth()/1.5f,-m_gameHeight/2.0f+temp->GetHeight()/1.5f));

	//m_background = new NoPhysicsObject(s_spriteManager.GetSprite("background"), m_gameWidth, m_gameHeight);
	
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	m_music = Mix_LoadMUS("Resources/Sounds/womanpuyang.mp3");
	
	//m_objects.push_back(new StandStillObject(s_spriteManager.GetSprite("b4"), 50, 50, PhysicsObject::BOX_SHAPE));
	m_steering =  new SteeringBehaviors(NULL);
	Sprite*sprite_snail = s_spriteManager.GetSprite("snail");
	for (int i = 0; i < 1; i++) {
		float x = RandomClamped(-(float)m_gameWidth / 2.0f, (float)m_gameWidth / 2.0f);
		float y = RandomClamped(-(float)m_gameHeight/ 2.0f, (float)m_gameHeight / 2.0f);
		float w = RandomClamped(40, 100);
		float h = w*(sprite_snail->GetHeight()/ sprite_snail->GetWidth());
		m_objects.push_back(new MovingObject(sprite_snail,0,0,w,h, PhysicsObject::BOX_SHAPE,m_steering));

		m_flock.push_back((MovingObject*)m_objects[m_objects.size() - 1]);
	}
	this->m_controller->SetSubject(m_flock[0]);


	// play music forever, fading in over 2 seconds
	// Mix_Music *music; // I assume this has been loaded already
	//if (m_music)
	//	if (Mix_FadeInMusic(m_music, 1, 2000) == -1)
	//		SDL_Log("Mix_FadeInMusic: %s\n", Mix_GetError());
		// well, there's no music, but most games don't break without music...

		//for (int i = 0; i < 100; i++)
		//sprites.push_back(new Sprite(s_resourceManager.GetTexture(0)));
	return 0;
}

void GameBase::Update(float deltaTime)
{
	float m_deltaTime = (float)(SDL_GetTicks() - m_frameStart)/1000.0f;
	m_frameStart = SDL_GetTicks();

	//float m_deltaTime = (float)m_frameDelay/1000.0f;

	s_physicsManager.Update(m_deltaTime);

	static float angle = 0;
	angle += m_deltaTime*60.0f;
	if (angle > 360.0f) {
		angle = 0.0f;
	}
	//SDL_Log("%f fps %f %f", 1.0f / m_deltaTime, m_deltaTime,deltaTime);


	m_mainCamera->GetController()->SetTarget(m_flock[0]->GetPos().x, m_flock[0]->GetPos().y);

	m_inputHolder->Update(m_deltaTime);
	m_controller->HandleEvent(m_inputHolder->GetTouch());
	m_mainCamera->Update(m_deltaTime);


	std::vector<Model3D*>models = *s_sceneManager.GetAllModels();
	for (int i = 0; i < s_sceneManager.GetLoadedModelNum(); i++) {
		models[i]->Update(m_deltaTime);
		models[i]->SetAngle(angle);
	}


	//m_background->SetSize(m_gameWidth+50.0f+glm::cos(glm::radians(angle)) * 100.0f, 50.0f+m_gameHeight+glm::cos(glm::radians(angle)) * 100.0f);

	this->m_gameWorld->Update(deltaTime);

	for (auto&object : m_objects) {
		object->Update(m_deltaTime);
	}
	
	m_controller->Update(m_deltaTime);
	

}

void GameBase::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//std::vector<Model3D*>models = *s_sceneManager.GetAllModels();
	//for (int i = 0; i < s_sceneManager.GetLoadedModelNum(); i++)
	//	models[i]->Draw();

	this->m_gameWorld->Draw();

	for(auto&object:m_objects)
		object->Draw();
	
	m_controller->Draw();

	//s_spriteManager.DrawLine(m_flock[0]->GetPos().x, m_flock[0]->GetPos().y, 
	//	m_flock[0]->GetPos().x+100.0f*m_flock[0]->GetHeading().x,
	//	m_flock[0]->GetPos().y + 100.0f*m_flock[0]->GetHeading().y);

	//for (int i = 0; i < sprites.size(); i++) 
	//	sprites[i]->Draw(0, 0, 500 - i * 20, 0, 100, 100);
	m_frameTime = SDL_GetTicks() - m_frameStart;
	if (m_frameDelay > m_frameTime) {
		SDL_Delay(m_frameDelay - m_frameTime);
		m_FPS = DEFAULT_FPS;
	}
	else
		m_FPS = 1000.0f/(float)m_frameTime;
}

void GameBase::CleanUp()
{
	for (auto&a : sprites)
		delete a;
	for (auto&object : m_objects)
		delete object;
	delete m_gameWorld;
	delete m_steering;
	delete m_controller;
	delete m_inputHolder;
	s_physicsManager.CleanUp();
	s_gameAccessor.CleanUp();
	s_sceneManager.CleanUp();
	s_resourceManager.CleanUp();
	s_inputDispatcher.CleanUp();
	s_spriteManager.CleanUp();

	Mix_FreeMusic(m_music);
	SDL_Log("Cleaned up everything");
}





#include"GameWorld\GameWorld.h"


int SmallTester::Init(int width, int height)
{

	GameWorld a;
	a.Init("Resources/Maps/map.txt");

	m_done = true;
	return 0;
}

void SmallTester::Update(float deltaTime)
{
}

void SmallTester::Draw()
{
}

void SmallTester::CleanUp()
{
}
