#include "GameBase.h"
#include<ctime>
#include<random>
#include"SDL2\SDL.h"
#include"SDL2\SDL_opengles2.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_mixer.h"
#include"SDL2\SDL_rwops.h"


#include"misc/Assistances.h"
#include"misc\Locator.h"
#include"AssetsManager.h"
#include"2D\TextureRenderer.h"
#include"2D\TextConsole.h"

#include"GameObjects\SpriterEntity.h"

#include"GameObjects/Character/Player.h"
#include"GameObjects/Character/Character.h"
#include"GameObjects/Character/CharacterBrain.h"
#include"GameObjects/Character/TerrestrialBody.h"

//#include<ft2build.h>
//#include FT_FREETYPE_H
GameBase::GameBase()
	:m_done(false)
{
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);//for loading files, logging, Timimg, etc 
	IMG_Init(IMG_INIT_PNG);//to load png image
	Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_FLAC);//music
	SDL_Log("SDL2 Initialized");
}


GameBase::~GameBase()
{
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_Log("Goodbye");
}

int GameBase::Init(int width, int height)
{
	m_windowSize.Init(width, height, 1920, 1080);
	m_timer.Init(50);
	m_camera2D.Init(m_windowSize.GetGameSize().w, m_windowSize.GetGameSize().h);
	SetupOpenGL(width, height);

	m_assetsManager.Init("Resources/assets.xml");
	TextureRenderer*textureRenderer = new TextureRenderer(m_assetsManager.GetShader("HelloTriangle"));
	TextConsole*textConsole = new TextConsole(textureRenderer, m_assetsManager.GetFontPath(0).c_str(), 50);// m_assetsManager.GetShader("HelloTriangle"));

	//textConsole->SetPos(-GAME_WIDTH / 2 + 200, GAME_HEIGHT / 2-200);
	textConsole->SetLineSpace(20);
	textConsole->SetCharSpace(5);
	textConsole->SetAlign(FreeTypeLoader::ALIGN_LEFT);
	textConsole->SetColor(1.0, 0.0, 1.0, 1.0);

	PrimitiveRenderer*primitiveRenderer = new PrimitiveRenderer(
		m_assetsManager.GetShader("PrimitiveDrawing"), 
		m_assetsManager.GetShader("PrimitiveFilling"),
		m_assetsManager.GetShader("2DLighting"));
	m_renderers.push_back(textureRenderer);
	m_renderers.push_back(primitiveRenderer);

	Locator::Provide(textureRenderer);
	Locator::Provide(textConsole);
	Locator::Provide(primitiveRenderer);
	Locator::Provide(&m_inputManager);
	Locator::Provide(&m_assetsManager);

	//m_animation = m_assetsManager.SpawnAnimationCollection("plant.png");
	//m_animation->SwitchAnimation("walk");

	m_world = new b2World(b2Vec2(0.0, -15.0));
	m_world->SetDebugDraw(&m_box2DRenderer);
	m_box2DRenderer.SetFlags(b2Draw::e_shapeBit);


	std::mt19937 generator(time(0));
	std::uniform_real_distribution<float> dimension(50.0f, 200.0f);
	std::uniform_real_distribution<float> pos(-5.0f, 5.0f);
	//for (int i = 0; i < 10; i++){
	//	float size = dimension(generator);
	//	m_entities.push_back(new Box(m_world, glm::vec2(pos(generator), pos(generator) / 2.0f), glm::vec2(size, size), m_assetsManager.GetTexture("tile")));
	//}
	//m_entities.push_back(new Player(m_world, glm::vec2(pos(generator), 
	//	pos(generator) / 2.0f), glm::vec2(80, 160), 1.2f*glm::vec2(80, 160),NULL));

	SpriterEntity*spriter = m_assetsManager.SpawnSpriterEntity("tree/deep/rocktree/entity_000");
		//new SpriterEntity(m_assetsManager.GetScmlObject("tree/deep/rocktree.scml"), 0);
	spriter->SetAnimation("dancing");
	m_entities.push_back(spriter);


	BodyBase*collisionBody = new TerrestrialBody(m_world,glm::vec2(0,100),glm::vec2(80,160));
	Skin*skin = new Skin(m_assetsManager.SpawnSpriterEntity("skin_2/main_skin/entity_000"), 1.0f);
	Brain*brain = new CharacterBrain(collisionBody,skin);
	m_entities.push_back(new Character(brain, collisionBody, skin));



	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(0.0f,-2.0f);//Example: if x is 100 then position.x is 1.0f
	b2Body*body = m_world->CreateBody(&bd);

	b2PolygonShape ps;
	ps.SetAsBox(15.0f,0.2f);

	b2FixtureDef fd;
	fd.shape = &ps;
	fd.restitution = 0.0f;
	body->CreateFixture(&fd);
	SDL_Log("Game Initialized");
	return 0;
}
void GameBase::SetupOpenGL(float width, float height)
{
	glViewport(
		m_windowSize.GetViewport().x, m_windowSize.GetViewport().y,
		m_windowSize.GetViewport().w, m_windowSize.GetViewport().h);

	//glClearColor(64.0f / 255.0f, 65.0f / 255.0f, 68.0f / 255.0f, 1.0f);
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SDL_Log("Opengl ES 2 Initialized");
}
void GameBase::HandleEvent(const InputEvent & inputEvent)
{

	if (inputEvent.type == KEY_DOWN) {
		m_inputManager.KeyDown(inputEvent.data.key);
	}
	if (inputEvent.type == KEY_UP) {
		m_inputManager.KeyUp(inputEvent.data.key);
	}
	if (inputEvent.type == MOUSE_MOTION|| inputEvent.type == MOUSE_BUTTON_DOWN|| inputEvent.type == MOUSE_BUTTON_UP) {
		//how to send mouse pos to world
		glm::vec2 pos1 = m_windowSize.ScreenToCamera(glm::vec2(inputEvent.data.motion.x, inputEvent.data.motion.y));
		glm::vec2 pos2 = m_camera2D.CameraToWorld(pos1);
		m_inputManager.SetMousePos(pos1);
		m_inputManager.SetMousePosInWorld(pos2);
	}
	if (inputEvent.type == MOUSE_BUTTON_DOWN) {
		m_inputManager.MouseDown();
	}
	if (inputEvent.type == MOUSE_BUTTON_UP) {
		m_inputManager.MouseUp();
	}
}

void GameBase::Update()
{
	m_timer.Start();
	float deltaTime = m_timer.GetDeltaTime();

	static float angle = 0; angle += deltaTime * 45; 
	if (angle > 360)angle = 0;
	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(50, 50, 50, 20), glm::radians(angle), glm::vec2(0,0));

	m_world->Step(deltaTime, 6, 2);
	m_camera2D.Update(deltaTime);

	//m_animation->Update(deltaTime);

	//if (m_inputManager.IsKeyPressed(KEY_ARROW_UP))
	//	m_camera2D.SetPos(m_camera2D.GetPosition()+glm::vec2(0,10));
	//if (m_inputManager.IsKeyPressed(KEY_ARROW_DOWN))
	//	m_camera2D.SetPos(m_camera2D.GetPosition() + glm::vec2(0, -10));
	//if (m_inputManager.IsKeyPressed(KEY_ARROW_LEFT))
	//	m_camera2D.SetPos(m_camera2D.GetPosition() + glm::vec2(-10,0.0f));
	//if (m_inputManager.IsKeyPressed(KEY_ARROW_RIGHT))
	//	m_camera2D.SetPos(m_camera2D.GetPosition() + glm::vec2(10,0.0f));

	for (auto&entity : m_entities)
		entity->Update(deltaTime);
}

void GameBase::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	Locator::GetTextConsole()->Log("Designed by VU DUY DU");
	Locator::GetTextConsole()->Log(std::string("FPS: ")+_to_string(m_timer.GetFPS()));

	Locator::GetPrimitiveRenderer()->DrawLight(Locator::GetInput()->GetMousePosInWorld(),
		700, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));

	Locator::GetPrimitiveRenderer()->DrawLine(glm::vec2(0, 0), Locator::GetInput()->GetMousePosInWorld());

	//m_world->DrawDebugData();


	for (auto&entity : m_entities) entity->Draw();
	Locator::GetTextConsole()->Draw();
	for(auto&renderer:m_renderers) renderer->Render(&(m_camera2D.GetMatrix()[0][0]));
	m_timer.End();
}

void GameBase::CleanUp()
{
	for (auto&renderer : m_renderers)
		delete renderer;
	for (auto&entity : m_entities)
		delete entity;

	if(m_world) delete m_world;

	delete Locator::GetTextConsole();
	m_assetsManager.CleanUp();
	SDL_Log("Cleaned up everything");
}




SmallTester::SmallTester() {

}


int SmallTester::Init(int width, int height)
{


	m_windowSize.Init(width, height, 1366, 768);
	m_timer.Init(50);
	m_camera2D.Init(m_windowSize.GetGameSize().w, m_windowSize.GetGameSize().h);
	SetupOpenGL(width, height);


	m_assetsManager.Init("Resources/assets.xml");
	TextureRenderer*textureRenderer = new TextureRenderer(m_assetsManager.GetShader("HelloTriangle"));
	PrimitiveRenderer*primitiveRenderer = new PrimitiveRenderer(
		m_assetsManager.GetShader("PrimitiveDrawing"),
		m_assetsManager.GetShader("PrimitiveFilling"),
		m_assetsManager.GetShader("2DLighting"));
	m_renderers.push_back(textureRenderer);
	m_renderers.push_back(primitiveRenderer);

	Locator::Provide(textureRenderer);
	Locator::Provide(primitiveRenderer);
	Locator::Provide(&m_inputManager);
	Locator::Provide(&m_assetsManager);




	SpriterEntity*spriter = m_assetsManager.SpawnSpriterEntity("tree/deep/rocktree/entity_000");
	spriter->SetAnimation("dancing");
	m_entities.push_back(spriter);
	return 0;
}

void SmallTester::Update()
{
	m_timer.Start();
	float deltaTime = m_timer.GetDeltaTime();
	m_camera2D.Update(deltaTime);
	for (auto&entity : m_entities) 
		entity->Update(deltaTime);
}


void SmallTester::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Locator::GetPrimitiveRenderer()->DrawLight(Locator::GetInput()->GetMousePosInWorld(),
		700, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
	Locator::GetPrimitiveRenderer()->DrawLine(glm::vec2(0, 0), Locator::GetInput()->GetMousePosInWorld());

	for (auto&entity : m_entities) entity->Draw();
	for (auto&renderer : m_renderers) renderer->Render(&(m_camera2D.GetMatrix()[0][0]));
	m_timer.End();
}

void SmallTester::CleanUp()
{
	for (auto&renderer : m_renderers)
		delete renderer;
	for (auto&entity : m_entities)
		delete entity;
	m_assetsManager.CleanUp();
	SDL_Log("Cleaned up everything");
}
