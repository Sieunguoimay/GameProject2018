#pragma once
#include<glm\glm.hpp>
class WindowSizeProblem
{

	glm::vec2 m_gameSize;
	glm::vec2 m_windowSize;

	float m_pixelToOpenGL;
	float m_deviceToGame;

	struct Viewport {float x, y, w, h;} m_viewport;

	glm::mat3 m_mat;
public:
	WindowSizeProblem();
	~WindowSizeProblem();
	void Init(int windowWidth, int windowHeight, int gameWidth, int gameHeight);

	Viewport&GetViewport() { return m_viewport; }
	const glm::vec2&GetGameSize() { return m_gameSize; }
	const glm::vec2&GetWindowSize() { return m_windowSize; }

	const glm::vec2& ScreenToCamera(const glm::vec2&point);
	const glm::mat3& GetMatrixScreenToCamera();
};

