#include "RealGame.h"

#include"GamePackage/misc/Assistances.h"
#include"GamePackage/misc/Locator.h"
#include"GamePackage/AssetsManager.h"

#include"GamePackage/GameObjects/Character/Character.h"
#include"GamePackage/GameObjects/Character/Animal.h"
#include"GamePackage/GameObjects/Character/Player/PlayerControl.h"
#include"GamePackage/GameObjects/World/World.h"
#include"GamePackage/GameObjects/ObjectPool.h"
#include"GamePackage\misc\HighLevelCamera.h"
#include"GamePackage\Input\MouseCursor.h"

#include"GamePackage\GameObjects\Character\MovableBox.h"

void RealGame::InitGameObjects()
{
	ObjectPool*objectPool = new ObjectPool(&m_entities);
	World*gameWorld = new World(objectPool);

	//Editor*editor = new Editor(gameWorld, objectPool);
	HighLevelCamera*camera = new HighLevelCamera(&m_camera2D, Locator::GetTextConsole()->GetFreeTypeLoader());;
	//InputHandler*inputHandler = new EditorPurposeHandler(editor, camera);
	InputHandler*inputHandler = new PlayerControl(objectPool);
	//inputHandler->AddCommand(camera, InputHandler::HT_CTRL);
	

	m_tools[OBJECT_POOL] = objectPool;
	//m_tools[EDITOR] = editor;
	m_tools[INPUT_HANDLER] = inputHandler;
	m_tools[WORLD] = gameWorld;
	m_tools[CAMERA_DRAG] = camera;
	m_tools[MOUSE_CURSOR] = new MouseCursor();

	m_entities.push_back(new MovableBox(new NoAnimationSkin(m_assetsManager.GetTexture("tree/deep/rock.png")), glm::vec4(0, 100, 100, 200)));

	for (int i = 0; i < TOOL_NUM; i++) if (m_tools[i]) m_tools[i]->Init();
	for (Node<AABBEntity*>*it = m_entities.first(); it != m_entities.tail; it = it->next) it->data->Init();

	/*static TouchPoint touchPoint;
	touchPoint.SetPos(b2Vec2(0, -1.5f));
	touchPoint.Init();
*/
	

	//Editor::SortEntitiesBySize(m_entities);//sort if you want to pick them up

										   //for the purpose of editing
										   //Locator::GetTextureRenderer()->Disable();
	//Locator::GetPrimitiveRenderer()->Disable();
	m_box2DRenderer.SetFlags(b2Draw::e_shapeBit);
	m_physicsFactory.SetRenderer(&m_box2DRenderer);
	AABBEntity::s_drawAABBTrigger = true;

	Locator::GetTextureRenderer()->CleanBuffer();//some entities init requiring calls of draw() function
	SDL_Log("Game Initialized");
}

void RealGame::Update()
{
	m_timer.Start();
	//TIMER::BEGIN: timer start to count
	float deltaTime = m_timer.GetDeltaTime();
	m_physicsFactory.Update(deltaTime);

	for (int i = 0; i < TOOL_NUM; i++) if (m_tools[i]) m_tools[i]->Update(deltaTime);
	for (Node<AABBEntity*>*it = m_entities.first(); it != m_entities.tail; it = it->next) {
		it->data->Update(deltaTime);

		//if (m_tools[EDITOR])((Editor*)m_tools[EDITOR])->GetPicker().Process(it->data, m_inputEvent.GetLeftMouse());

		if (it->data->HasDone()) {
			delete it->data;
			it = m_entities.erase(it);
		}
	}

	m_camera2D.Update(deltaTime);
	//INPUTEVENT::END: range of input event Usage
	m_inputEvent.ClearEvent();
}

