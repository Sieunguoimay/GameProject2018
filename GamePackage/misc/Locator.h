#pragma once
#include"../Input/InputManager.h"
#include"../2D/PrimitiveRenderer.h"
#include"../2D/TextureRenderer.h"
#include"../2D/TextConsole.h"
#include"../AssetsManager.h"
//service locator
class Locator
{
	//primitive renderer
	static PrimitiveRenderer* s_pPrimitiveRenderer;
	static TextureRenderer*s_pTextureRenderer;
	static TextConsole*s_pTextConsole;
	//input
	static InputManager* s_pInput;

	//audio goes here

	//assets
	static AssetsManager*s_pAssets;
public:
	Locator();
	~Locator();

	static void Provide(InputManager* input) { s_pInput = input; }
	static void Provide(PrimitiveRenderer* renderer) { s_pPrimitiveRenderer = renderer; }
	static void Provide(TextureRenderer* renderer) { s_pTextureRenderer = renderer; }
	static void Provide(TextConsole* console) { s_pTextConsole = console; }
	static void Provide(AssetsManager* assets) { s_pAssets = assets; }

	static InputManager*GetInput() { return s_pInput; }
	static PrimitiveRenderer*GetPrimitiveRenderer() { return s_pPrimitiveRenderer; }
	static TextureRenderer*GetTextureRenderer() { return s_pTextureRenderer; }
	static TextConsole*GetTextConsole() { return s_pTextConsole; }
	static AssetsManager*GetAssets() { return s_pAssets; }
};

