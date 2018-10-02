// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include"GameBase.h"
#include "../Utilities/utilities.h"
GameBase*g_gameBase;
bool running = true;
int Init(ESContext *esContext)
{
	g_gameBase->Init(esContext->width, esContext->height);
	running = true;
	return 0;
}

void Draw(ESContext *esContext)
{
	g_gameBase->Draw();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

bool Update(ESContext *esContext, float deltaTime)
{
	g_gameBase->Update(deltaTime);
	return g_gameBase->Done();
}

void HandleEvent(ESContext *esContext, int type,int key, int x, int y)
{
	if (type == WM_KEYDOWN) {
		if(key== 65)//a
				s_inputDispatcher.SendEvent(SDL_KEYDOWN, SDLK_a, DEFAULT);
		if (key == 83)//d
				s_inputDispatcher.SendEvent(SDL_KEYDOWN, SDLK_s, DEFAULT);
		if (key == 68)//s
				s_inputDispatcher.SendEvent(SDL_KEYDOWN, SDLK_d, DEFAULT);
		if (key == 87)//w
				s_inputDispatcher.SendEvent(SDL_KEYDOWN, SDLK_w, DEFAULT);
		if (key == 32)//space
				s_inputDispatcher.SendEvent(SDL_KEYDOWN, SDLK_SPACE, DEFAULT);
		if (key == 13)//enter
				s_inputDispatcher.SendEvent(SDL_KEYDOWN, SDLK_RETURN, DEFAULT);
		if (key == 27) {//escape
			s_inputDispatcher.SendEvent(SDL_KEYDOWN, SDLK_ESCAPE, DEFAULT);
			running = false;
		}
		//SDL_Log("KEY DOWN %d", (int)key);
	}else if (type == WM_KEYUP) {
		if (key == 65)//a
			s_inputDispatcher.SendEvent(SDL_KEYUP, SDLK_a, DEFAULT);
		if (key == 83)//S
			s_inputDispatcher.SendEvent(SDL_KEYUP, SDLK_s, DEFAULT);
		if (key == 68)//D
			s_inputDispatcher.SendEvent(SDL_KEYUP, SDLK_d, DEFAULT);
		if (key == 87)//w
			s_inputDispatcher.SendEvent(SDL_KEYUP, SDLK_w, DEFAULT);
		if (key == 32)//space
			s_inputDispatcher.SendEvent(SDL_KEYUP, SDLK_SPACE, DEFAULT);
		if (key == 13)//enter
			s_inputDispatcher.SendEvent(SDL_KEYUP, SDLK_RETURN, DEFAULT);
		if (key == 27)//escape
			s_inputDispatcher.SendEvent(SDL_KEYUP, SDLK_ESCAPE, DEFAULT);
		//SDL_Log("KEY UP %d", (int)key);
	}
	if (type == WM_MOUSEMOVE) {
		s_inputDispatcher.SendEvent(SDL_MOUSEMOTION, x, y);
	}
	if (type == WM_LBUTTONDOWN) {
		s_inputDispatcher.SendEvent(SDL_MOUSEBUTTONDOWN, x, y);
	}
	if (type == WM_LBUTTONUP) {
		s_inputDispatcher.SendEvent(SDL_MOUSEBUTTONUP, x, y);
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
	esCreateWindow(&esContext, "Hello Triangle", 1366, 768, ES_WINDOW_ALPHA | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, HandleEvent);

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	//printf("Press any key...\n");
	_getch();

	return 0;
}

