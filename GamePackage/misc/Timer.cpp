#include "Timer.h"

void Timer::Init(float fps)
{
	//timming
	this->m_FPS = fps;
	this->m_frameDelay = 1000 / m_FPS;
	this->m_frameStart = SDL_GetTicks();
}

void Timer::Start()
{
	m_deltaTime = (float)(SDL_GetTicks() - m_frameStart) / 1000.0f;
	m_frameStart = SDL_GetTicks();
}

void Timer::End() {
	m_frameTime = SDL_GetTicks() - m_frameStart;
	if (m_frameDelay > m_frameTime) {
		SDL_Delay(m_frameDelay - m_frameTime);
		m_FPS = 1000.0f / (float)m_frameDelay;
	}
	else
		m_FPS = 1000.0f / (float)m_frameTime;

	//SDL_Log("%f", m_FPS);
}