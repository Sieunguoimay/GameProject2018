// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include"GamePackage/GameBase.h"
#include "../Utilities/utilities.h"
GameBase*g_gameBase;
bool running = true;
int Init(ESContext *esContext)
{
	running = true;
	return g_gameBase->Init(esContext->width, esContext->height);
}

void Draw(ESContext *esContext)
{
	g_gameBase->Draw();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

bool Update(ESContext *esContext, float deltaTime)
{
	g_gameBase->Update();
	return g_gameBase->Done();
}

void HandleEvent(ESContext *esContext, int type,int key, int x, int y)
{
	if (type == WM_KEYDOWN) {
		if(key== 65)//a
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_A, Key::NULL_KEY));
		if (key == 83)//s
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_S, Key::NULL_KEY));
		if (key == 68)//d
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_D, Key::NULL_KEY));
		if (key == 87)//w
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_W, Key::NULL_KEY));
		if (key == 32)//space
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_SPACE, Key::NULL_KEY));
		if (key == 13)//enter
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_ENTER, Key::NULL_KEY));
		if (key == 27) {//escape
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_ESCAPE, Key::NULL_KEY));
		}
		if (key == 38)//w
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_ARROW_UP, Key::NULL_KEY));
		if (key == 40)//space
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_ARROW_DOWN, Key::NULL_KEY));
		if (key == 37)//enter
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_ARROW_LEFT, Key::NULL_KEY));
		if (key == 39) {//escape
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_DOWN, Key::KEY_ARROW_RIGHT, Key::NULL_KEY));
		}
		//SDL_Log("KEY DOWN %d", (int)key);
	}else if (type == WM_KEYUP) {
		if (key == 65)//a
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_A, Key::NULL_KEY));
		if (key == 83)//s
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_S, Key::NULL_KEY));
		if (key == 68)//d
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_D, Key::NULL_KEY));
		if (key == 87)//w
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_W, Key::NULL_KEY));
		if (key == 32)//space
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_SPACE, Key::NULL_KEY));
		if (key == 13)//enter
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_ENTER, Key::NULL_KEY));
		if (key == 27) {//escape
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_ESCAPE, Key::NULL_KEY));
		}
		if (key == 38)//up
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_ARROW_UP, Key::NULL_KEY));
		if (key == 40)//down
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_ARROW_DOWN, Key::NULL_KEY));
		if (key == 37)//left
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_ARROW_LEFT, Key::NULL_KEY));
		if (key == 39) {//right
			g_gameBase->HandleEvent(InputEvent(EventType::KEY_UP, Key::KEY_ARROW_RIGHT, Key::NULL_KEY));
		}
		//SDL_Log("KEY UP %d", (int)key);
	}
	if (type == WM_MOUSEMOVE) {
		g_gameBase->HandleEvent(InputEvent(EventType::MOUSE_MOTION, x, y));
	}
	if (type == WM_LBUTTONDOWN) {
		g_gameBase->HandleEvent(InputEvent(EventType::MOUSE_BUTTON_DOWN, x, y));
	}
	if (type == WM_LBUTTONUP) {
		g_gameBase->HandleEvent(InputEvent(EventType::MOUSE_BUTTON_UP, x, y));
	}
}

void CleanUp()
{
	g_gameBase->CleanUp();
	delete g_gameBase;
}
int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

	g_gameBase = new GameBase();//SmallTester();//

	esInitContext(&esContext);
	esCreateWindow(&esContext, "Hello Triangle", 900, 600, ES_WINDOW_ALPHA | ES_WINDOW_DEPTH);

	if (Init(&esContext) == 0) {
		DisplayMemoryInfo();

		esRegisterDrawFunc(&esContext, Draw);
		esRegisterUpdateFunc(&esContext, Update);
		esRegisterKeyFunc(&esContext, HandleEvent);

		esMainLoop(&esContext);

		DisplayMemoryInfo();

		//releasing OpenGL resources
		CleanUp();


		//identifying memory leaks
		MemoryDump();
		//printf("Press any key...\n");
	}
	_getch();

	return 0;
}

