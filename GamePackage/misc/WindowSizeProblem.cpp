#include "WindowSizeProblem.h"



WindowSizeProblem::WindowSizeProblem()
{
}


WindowSizeProblem::~WindowSizeProblem()
{
}

void WindowSizeProblem::Init(int windowWidth, int windowHeight, int gameWidth, int gameHeight)
{
	m_windowSize.w = windowWidth;
	m_windowSize.h = windowHeight;
	m_gameSize.w = gameWidth;
	m_gameSize.h = gameHeight;


	float gameSizeOrPerspectiveRatio = (float)gameWidth / (float)gameHeight;

	float viewportX = 0.0f;
	float viewportY = 0.0f;
	float viewportW;
	float viewportH;

	float windowRatio = (float)windowWidth / (float)windowHeight;
	if (windowRatio < gameSizeOrPerspectiveRatio) {//window width is smaller
		viewportW = windowWidth;
		viewportH = windowWidth / gameSizeOrPerspectiveRatio;
		viewportY = (windowHeight - viewportH) / 2.0f;
		m_deviceToGame = (float)gameWidth/(float)windowWidth;//multiply device position with this to get postion in game wrt the origin
	}
	else {
		viewportW = windowHeight * gameSizeOrPerspectiveRatio;
		viewportH = windowHeight;
		viewportX = (windowWidth - viewportW) / 2.0f;
		m_deviceToGame = (float)gameHeight / (float)windowHeight;//multiply device position with this to get postion in game wrt the origin
	}


	m_viewport.x = viewportX;
	m_viewport.y = viewportY;
	m_viewport.w = viewportW;
	m_viewport.h = viewportH;

	if (gameSizeOrPerspectiveRatio > 1.0f) {
		m_pixelToOpenGL = 1.0f / gameHeight;
	}
	else {	//never happens, in case perspective can accept this ratio
		m_pixelToOpenGL = 1.0f / gameWidth;
	}
}

glm::vec2 WindowSizeProblem::ScreenToCamera(glm::vec2 point) {
	glm::vec2 pw;
	pw.x = (point.x - m_viewport.x)*m_deviceToGame - m_gameSize.w / 2.0f;
	pw.y = -(point.y - m_viewport.y)*m_deviceToGame + m_gameSize.h / 2.0f;
	return pw;

}

