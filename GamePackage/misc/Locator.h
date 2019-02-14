#pragma once
#include"../Input/InputManager.h"
#include"../Input/Controller.h"
#include"../2D/PrimitiveRenderer.h"
#include"../2D/TextureRenderer.h"
#include"../2D/TextConsole.h"
#include"../2D/Camera2D.h"
#include"../AssetsManager.h"
#include"../GameObjects/PhysicsEngine/PhysicsFactory.h"
//service locator
class Locator
{
	//primitive renderer
	static PrimitiveRenderer* s_pPrimitiveRenderer;
	static TextureRenderer*s_pTextureRenderer;
	static TextConsole*s_pTextConsole;
	//input
	static InputManager* s_pInput;
	static Controller* s_pController;
	//audio goes here

	//assets
	static AssetsManager*s_pAssets;

	//camera
	static Camera2D*s_pCamera;

	//physics
	static PhysicsFactory*s_physicsFactory;
public:
	Locator();
	~Locator();

	static void Provide(InputManager* input) { s_pInput = input; }
	static void Provide(Controller* controller) { s_pController = controller; }
	static void Provide(PrimitiveRenderer* renderer) { s_pPrimitiveRenderer = renderer; }
	static void Provide(TextureRenderer* renderer) { s_pTextureRenderer = renderer; }
	static void Provide(TextConsole* console) { s_pTextConsole = console; }
	static void Provide(AssetsManager* assets) { s_pAssets = assets; }
	static void Provide(Camera2D*camera) { s_pCamera = camera; }
	static void Provide(PhysicsFactory*physicsFactory) { s_physicsFactory = physicsFactory; }

	static InputManager*GetInput() { return s_pInput; }
	static Controller*GetController() { return s_pController; }
	static PrimitiveRenderer*GetPrimitiveRenderer() { return s_pPrimitiveRenderer; }
	static TextureRenderer*GetTextureRenderer() { return s_pTextureRenderer; }
	static TextConsole*GetTextConsole() { return s_pTextConsole; }
	static AssetsManager*GetAssets() { return s_pAssets; }
	static Camera2D*GetCamera() { return s_pCamera; }
	static PhysicsFactory*GetPhysicsFactory() { return s_physicsFactory; }
};

#define s_controller Locator::GetController()
