#pragma once
#include<glm\glm.hpp>
class WindowSizeProblem
{

	struct Size { int w, h; };
	Size m_gameSize;
	Size m_windowSize;

	float m_pixelToOpenGL;
	float m_deviceToGame;

	struct Viewport {float x, y, w, h;} m_viewport;

	glm::mat3 m_mat;
public:
	WindowSizeProblem();
	~WindowSizeProblem();
	void Init(int windowWidth, int windowHeight, int gameWidth, int gameHeight);

	Viewport&GetViewport() { return m_viewport; }
	Size&GetGameSize() { return m_gameSize; }
	Size&GetWindowSize() { return m_windowSize; }

	const glm::vec2& ScreenToCamera(const glm::vec2&point);
	const glm::mat3& GetMatrixScreenToCamera();
};

