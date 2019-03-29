#pragma once
#include<SDL2\SDL.h>
class Timer
{
	Uint32 m_frameStart;
	float m_deltaTime;
	int m_frameTime;
	float m_FPS;
	int m_frameDelay;
public:
	void Init(float fps);

	void Start();
	void End();

	inline const float& GetDeltaTime() { return m_deltaTime; }
	inline const float& GetFPS() { return m_FPS; }
};

