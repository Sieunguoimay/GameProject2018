#include "GameBase.h"
#include<ctime>
#include<random>
#include"SDL2\SDL.h"
#include"SDL2\SDL_opengles2.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_mixer.h"
#include"SDL2\SDL_rwops.h"
#include"SDL2\SDL_ttf.h"

#include"misc/Assistances.h"
#include"misc\Locator.h"
#include"AssetsManager.h"
#include"2D\TextureRenderer.h"
#include"2D\TextConsole.h"

#include"GameObjects\SpriterEntity.h"

#include"GameObjects/Character/Character.h"
#include"GameObjects/Character/CharacterBrain.h"
#include"GameObjects/Character/TerrestrialBody.h"
#include"GameObjects/Character/Animal.h"
#include"GameObjects/ObjectPool.h"

#include"GameObjects/PhysicsEngine/PhysicsFactory.h"

#include"Input/InputHandler.h"
#include"Input/MouseCursor.h"

#include"misc/Editor/EditorPurposeHandler.h"


GameBase::GameBase()
	:m_done(false)
{
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);//for loading files, logging, Timimg, etc 
	IMG_Init(IMG_INIT_PNG);//to load png image
	Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_FLAC);//music
	TTF_Init();
	SDL_Log("SDL2 Initialized");
}


GameBase::~GameBase()
{
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_Log("Goodbye");
}

int GameBase::Init(int width, int height)
{
	m_windowSize.Init(width, height, 2400, 1348);
	m_camera2D.Init(m_windowSize.GetGameSize(), m_windowSize.GetWindowSize());
	m_timer.Init(50);
	setupOpenGL(width, height);

	m_assetsManager.Init("Resources/assets.xml");

	TextureRenderer*textureRenderer = new TextureRenderer(m_assetsManager.GetShader("HelloTriangle"),SortType::TEXTURE,m_camera2D.GetAABB());
	PrimitiveRenderer*primitiveRenderer = new PrimitiveRenderer(m_assetsManager.GetShader("PrimitiveDrawing"),m_assetsManager.GetShader("PrimitiveFilling"),m_assetsManager.GetShader("2DLighting"));
	TextConsole*textConsole = new TextConsole(textureRenderer, m_assetsManager.GetFontPath(1).c_str(), 50);
	textConsole->SetPos(-m_windowSize.GetGameSize().x/ 2 + 500, m_windowSize.GetGameSize().y / 2 - 200);
	m_renderers.push_back(textConsole);//at first
	m_renderers.push_back(textureRenderer);
	m_renderers.push_back(primitiveRenderer);

	m_physicsFactory.Init();

	Locator::Provide(textureRenderer);
	Locator::Provide(textConsole);
	Locator::Provide(primitiveRenderer);
	Locator::Provide(&m_inputEvent);
	Locator::Provide(&m_assetsManager);
	Locator::Provide(&m_physicsFactory);

	InitGameObjects();

	return 0;
}
void GameBase::setupOpenGL(float width, float height)
{
	glViewport(
		m_windowSize.GetViewport().x, m_windowSize.GetViewport().y,
		m_windowSize.GetViewport().w, m_windowSize.GetViewport().h);

	//glClearColor(64.0f / 255.0f, 65.0f / 255.0f, 68.0f / 255.0f, 1.0f);
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	//glClearColor(0.9f, 0.0f, 0.9f, 1.0f);
	glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SDL_Log("Opengl ES 2 Initialized");
}

/*
BodyBase*collisionBody = new TerrestrialBody(glm::vec2(0, 100), glm::vec2(80, 160));
AnimationSkin*skin = new AnimationSkin(m_assetsManager.SpawnSpriterEntity("skin_2/main_skin/entity_000"), 1.0f);
Brain*brain = new CharacterBrain(collisionBody, skin);
m_entities.push_back(new Character(brain, collisionBody, skin));

std::mt19937 generator(time(0));
std::uniform_real_distribution<float> dimension(50.0f, 200.0f);
*/


void GameBase::InitGameObjects()
{
	ObjectPool*objectPool = new ObjectPool(&m_entities);
	World*gameWorld = new World(objectPool);
	HighLevelCamera*camera = new HighLevelCamera(&m_camera2D,Locator::GetTextConsole()->GetFreeTypeLoader());;

	//for the purpose of editing
	//Locator::GetTextureRenderer()->Disable();
	//Locator::GetPrimitiveRenderer()->Disable();
	m_box2DRenderer.SetFlags(b2Draw::e_shapeBit);
	m_physicsFactory.SetRenderer(&m_box2DRenderer);
	SpriteTimelineKey::s_drawAABBTrigger = false;
	AABBEntity::s_drawAABBTrigger = true;

	Editor*editor = new Editor(gameWorld, objectPool);
	InputHandler*inputHandler = new EditorPurposeHandler(editor, camera);
	m_tools[EDITOR] = editor;
	m_tools[INPUT_HANDLER] = inputHandler;


	m_tools[OBJECT_POOL] = objectPool;
	m_tools[WORLD] = gameWorld;
	m_tools[CAMERA_DRAG] = camera;
	m_tools[MOUSE_CURSOR] = new MouseCursor();


	for (int i = 0; i < TOOL_NUM; i++) if (m_tools[i]) m_tools[i]->Init();
	//Editor::LoadGameWorldFromXml("Resources/GameData/game_world_data.xml", gameWorld, objectPool, NULL);
	for (Node<AABBEntity*>*it = m_entities.first(); it != m_entities.tail; it = it->next) it->data->Init();
	Editor::SortEntitiesBySize(m_entities);//sort if you want to pick them up
	Locator::GetTextureRenderer()->CleanBuffer();//some entities init requiring calls of draw() function
	SDL_Log("Game Initialized");
}

void GameBase::HandleEvent(const InputEventPackage & inputEvent)
{
	m_inputEvent.ProcessInputEvent(inputEvent, 
		m_windowSize.GetMatrixScreenToCamera(),
		m_camera2D.GetPosition());

	//INPUTEVENT::BEGIN: range of input event Usage
}

void GameBase::Update()
{
	m_timer.Start();
	//TIMER::BEGIN: timer start to count
	float deltaTime = m_timer.GetDeltaTime();
	m_physicsFactory.Update(deltaTime);

	for (int i = 0; i < TOOL_NUM; i++) if(m_tools[i]) m_tools[i]->Update(deltaTime);
	for (Node<AABBEntity*>*it = m_entities.first(); it != m_entities.tail; it = it->next){
		it->data->Update(deltaTime);

		if(m_tools[EDITOR])((Editor*)m_tools[EDITOR])->GetPicker().Process(it->data,m_inputEvent.GetLeftMouse());

		if (it->data->HasDone()) {
			delete it->data;
			it=m_entities.erase(it);
		}
	}

	m_camera2D.Update(deltaTime);
	//INPUTEVENT::END: range of input event Usage
	m_inputEvent.ClearEvent();
}

void GameBase::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//RENDERER::BEGIN: draw everything here....
	for (int i = 0; i < TOOL_NUM; i++) if (m_tools[i]) m_tools[i]->Draw();
	for (Node<AABBEntity*>*it = m_entities.first(); it != m_entities.tail; it = it->next) it->data->Draw();

	//Locator::GetTextConsole()->Log("Designed by VU DUY DU");
	//Locator::GetPrimitiveRenderer()->DrawLight(Locator::GetInput()->GetMousePosInWorld(),
	//	700, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
	//	glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
	Locator::GetTextConsole()->Log(("FPS: "+std::to_string((int)m_timer.GetFPS())+"\nTEX: "+ std::to_string(Locator::GetTextureRenderer()->GetDrawNum())).c_str(),true);

	//RENDERER::END: push them up to the sky
	for(auto&renderer:m_renderers) renderer->Render(&(m_camera2D.GetMatrix()[0][0]));
	//TIMMER::END: timer stops
	m_timer.End();
}

void GameBase::CleanUp()
{
	for (Node<AABBEntity*>*it = m_entities.first(); it != m_entities.tail; it = it->next){
		delete it->data;
		it= m_entities.erase(it);
	}
	for (int i = 0; i < TOOL_NUM; i++)	if(m_tools[i]) delete m_tools[i];
	for (auto&renderer : m_renderers)	delete renderer;

	m_physicsFactory.CleanUp();
	m_assetsManager.CleanUp();

	SDL_Log("Cleaned up everything");
}























SmallTester::SmallTester() {

}


int SmallTester::Init(int width, int height)
{
	GameBase::Init(width, height);
	return 0;
}
void SmallTester::InitGameObjects()
{

}
void SmallTester::Update()
{
	m_timer.Start();
	float deltaTime = m_timer.GetDeltaTime();


	//for (auto&entity : m_entities) {
	//	entity->Update(deltaTime);
	//}

	m_camera2D.Update(deltaTime);
}


void SmallTester::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//for (auto&entity : m_entities) entity->Draw();

	char s[256];
	sprintf(s, "FPS: %d\n%d", (int)m_timer.GetFPS(), Locator::GetTextureRenderer()->GetDrawNum());
	Locator::GetTextConsole()->Log(s);

	for (auto&renderer : m_renderers) renderer->Render(&(m_camera2D.GetMatrix()[0][0]));
	m_timer.End();
}

void SmallTester::CleanUp()
{
	for (auto&renderer : m_renderers)
		delete renderer;
	//for (auto&entity : m_entities)
	//	delete entity;


	delete Locator::GetTextConsole();
	m_assetsManager.CleanUp();
	SDL_Log("Cleaned up everything");
}
