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

#include"GameObjects\PhysicsEngine\PhysicsFactory.h"

#include"GameObjects\PhysicsEngine\TestBox2d.h"


#include"GameObjects\Character\Animal.h"

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
	m_timer.Init(50);
	m_camera2D.Init(m_windowSize.GetGameSize().w, m_windowSize.GetGameSize().h);
	SetupOpenGL(width, height);

	m_assetsManager.Init("Resources/assets.xml");

	TextureRenderer*textureRenderer = new TextureRenderer(m_assetsManager.GetShader("HelloTriangle"),SortType::TEXTURE);

	PrimitiveRenderer*primitiveRenderer = new PrimitiveRenderer(
		m_assetsManager.GetShader("PrimitiveDrawing"), 
		m_assetsManager.GetShader("PrimitiveFilling"),
		m_assetsManager.GetShader("2DLighting"));
	m_renderers.push_back(textureRenderer);
	m_renderers.push_back(primitiveRenderer);
	
	m_box2DRenderer.SetFlags(b2Draw::e_shapeBit);

	TextConsole*textConsole = new TextConsole(textureRenderer, m_assetsManager.GetFontPath(1).c_str(), 50);// m_assetsManager.GetShader("HelloTriangle"));
	textConsole->SetPos(-GAME_WIDTH / 2 + 200, GAME_HEIGHT / 2 - 200);


	m_physicsFactory.Init();
	m_physicsFactory.SetRenderer(&m_box2DRenderer);


	Locator::Provide(textureRenderer);
	Locator::Provide(textConsole);
	Locator::Provide(primitiveRenderer);
	Locator::Provide(&m_controller);
	Locator::Provide(&m_inputEvent);
	Locator::Provide(&m_assetsManager);
	Locator::Provide(&m_camera2D);
	Locator::Provide(&m_physicsFactory);


	InitGameObjects();

	return 0;
}
void GameBase::SetupOpenGL(float width, float height)
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


void GameBase::InitGameObjects()
{


	//Locator::GetTextureRenderer()->Disable();
	//Locator::GetPrimitiveRenderer()->Disable();

	World*gameWorld = new World();
	//Editor::LoadObjectFromPEXml("Resources/RawData/platform3.xml",gameWorld);
	//Editor::LoadObjectFromPEXml("Resources/RawData/platform2.xml",gameWorld);
	//gameWorld->AddPlatform(Editor::LoadObjectFromJson("Resources/RawData/demo.json"));
	//Editor::LoadGameWorldFromXml("Resources/GameData/game_world_data.xml", gameWorld);
	m_entities.push_back(gameWorld);


	m_editor.Init(gameWorld,"");
	m_tools.push_back(&m_editor);

/*
	BodyBase*collisionBody = new TerrestrialBody(glm::vec2(0, 100), glm::vec2(80, 160));
	AnimationSkin*skin = new AnimationSkin(m_assetsManager.SpawnSpriterEntity("skin_2/main_skin/entity_000"), 1.0f);
	Brain*brain = new CharacterBrain(collisionBody, skin);
	m_entities.push_back(new Character(brain, collisionBody, skin));

	m_camera2D.SetTarget(collisionBody->GetPosPointer());
*/
	m_entities.push_back(new Animal(glm::vec2(500,0)));
	m_entities.push_back(new Rock());
	m_entities.push_back(new Grass());
	/*m_entities.push_back(new Leaf());
	m_entities.push_back(new FlockOfAnimal());
*/
	AABBEntity::ShowAABB();


	std::mt19937 generator(time(0));
	std::uniform_real_distribution<float> dimension(50.0f, 200.0f);
	std::uniform_real_distribution<float> pos(-5.0f, 5.0f);

	//SpriterEntity*spriter = m_assetsManager.SpawnSpriterEntity("tree/deep/rocktree/entity_000");
	//spriter->SetAnimation("dancing");
	//m_entities.push_back(spriter);

	//for (int i = 0; i < 15; i++) {
	//	spriter = m_assetsManager.SpawnSpriterEntity("tree/deep/rocktree/entity_000");
	//	spriter->SetAnimation("standing");
	//	m_entities.push_back(spriter);
	//}

	//for (int i = 0; i < 6; i++)
	//{
	//	spriter = m_assetsManager.SpawnSpriterEntity("skin_2/main_skin/entity_000");
	//	spriter->SetAnimation("run");
	//	m_entities.push_back(spriter);
	//}
	b2PolygonShape ps;
	ps.SetAsBox(10, 0.2);
	Locator::GetPhysicsFactory()->CreateBody(&ps, b2_staticBody, MaterialType::SOIL, b2Vec2(0, -4.75));

	SDL_Log("Game Initialized");
}

void GameBase::HandleEvent(const InputEventDeliver & inputEvent)
{
	m_inputEvent.ProcessInputEvent(inputEvent, 
		m_windowSize.GetMatrixScreenToCamera(),
		m_camera2D.GetPosition());

}

void GameBase::Update()
{
	m_timer.Start();
	float deltaTime = m_timer.GetDeltaTime();
	m_physicsFactory.Update(deltaTime);
	//SDL_Log("FPS: %f", m_timer.GetFPS());
	//static float angle = 0; angle += deltaTime * 45; 
	//if (angle > 360)angle = 0;
	//Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(50, 50, 50, 20), glm::radians(angle), glm::vec2(0,0));

	m_controller.Update(&m_inputEvent);

	for (auto&tool : m_tools) tool->Update(deltaTime);

	for (auto&entity : m_entities) {
		entity->Update(deltaTime);
		m_editor.GetPicker().Process(entity,m_inputEvent);
	}
	m_camera2D.Update(deltaTime);
}

void GameBase::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Locator::GetTextConsole()->Log("Designed by VU DUY DU");

	//Locator::GetPrimitiveRenderer()->DrawLight(Locator::GetInput()->GetMousePosInWorld(),
	//	700, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
	//	glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));


	Locator::GetPrimitiveRenderer()->DrawCircle(Locator::GetInputEvent()->GetMousePosInWorld()-glm::vec2(2,2),
		4,glm::vec4(1.0,0.0,1.0,1.0));

	//SDL_Log("FPS: %f", m_timer.GetFPS());

	//for (int i = 0; i < 115; i++)
	//	Locator::GetTextureRenderer()->Draw(
	//		glm::vec4(100, 10, 100, 100),		//quad mesh
	//		glm::vec4(0, 0, 1, 1),				// UVs
	//		i%(m_assetsManager.GetTextureNum())	//texture id
	//		, 0									//angle
	//		, glm::vec4(1.0f));					//color



	//int n = m_assetsManager.GetTextureNum();
	//for (int i = 0; i < n; i++)
	//	Locator::GetTextureRenderer()->Draw(
	//		glm::vec4(-900+i, 100, 100, 100),
	//		glm::vec4(0.0, 0.0, 1.0, 1.0),
	//		n - i % n , 0, glm::vec4(1.0f)
	//		);



	for (auto&tool : m_tools) tool->Draw();
	for (auto&entity : m_entities) entity->Draw();

	char s[256];
	sprintf(s, "FPS: %d\n%d", (int)m_timer.GetFPS(), 
		Locator::GetTextureRenderer()->GetDrawNum());
	Locator::GetTextConsole()->Log(s);



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


	delete Locator::GetTextConsole();

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

	m_controller.Update(Locator::GetInputEvent());

	for (auto&entity : m_entities) {
		entity->Update(deltaTime);
	}

	m_camera2D.Update(deltaTime);
}


void SmallTester::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto&entity : m_entities) entity->Draw();

	char s[256];
	sprintf(s, "FPS: %d\n%d", (int)m_timer.GetFPS(), Locator::GetTextureRenderer()->GetDrawNum());
	Locator::GetTextConsole()->Log(s);

	Locator::GetTextConsole()->Draw();

	for (auto&renderer : m_renderers) renderer->Render(&(m_camera2D.GetMatrix()[0][0]));
	m_timer.End();
}

void SmallTester::CleanUp()
{
	for (auto&renderer : m_renderers)
		delete renderer;
	for (auto&entity : m_entities)
		delete entity;


	delete Locator::GetTextConsole();
	m_assetsManager.CleanUp();
	SDL_Log("Cleaned up everything");
}
