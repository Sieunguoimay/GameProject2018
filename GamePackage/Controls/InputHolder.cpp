#include"InputHolder.h"
#include"SDL2\SDL.h"
//InputHolder*InputHolder::s_instance = 0;
//InputHolder&InputHolder::Instance() {
//	if (s_instance == 0)
//		s_instance = new InputHolder();
//	return *s_instance;
//}
InputHolder::InputHolder() 
	:m_keyPressed(false),
	m_keyDown(false),
	m_keyUp(false),
	m_mouseX(0),m_mouseY(0),
	m_clicked(false),
	m_released(false),
	m_moving(false),
	m_controller(NULL)
{

}

InputHolder::~InputHolder()
{
	CleanUp();
}


void InputHolder::CleanUp() {
}

void InputHolder::HandleEvent(Event&e) {
	//reset something that lives for one loop cycle
	if (m_touch.up)
		m_touch.up = false;



	//new event process here..
	if (e.type == SDL_KEYDOWN) {
		m_key.keys[e.arg1] = true;
		if(!m_keyDown&&!m_keyPressed)
			m_keyDown = true;
		m_keyPressed = true;
	}
	else if (e.type == SDL_KEYUP) {
		m_key.keys[e.arg1] = false;
		m_keyUp = true;
		m_keyPressed = false;
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		m_clicked = true;
		m_mouseX = e.arg1;
		m_mouseY = e.arg2;

		if (!m_touch.touching) {
			m_touch.touching = true;
			m_touch.x = e.arg1;
			m_touch.y = e.arg2;
			m_touch.cur_x = e.arg1;
			m_touch.cur_y = e.arg2;
		}
	}

	if (e.type == SDL_MOUSEBUTTONUP) {
		m_released = true;
		m_mouseX = e.arg1;
		m_mouseY = e.arg2;

		m_touch.touching = false;
		m_touch.up = true;
	}
	if (e.type == SDL_MOUSEMOTION) {
		m_moving = true;
		m_mouseX = e.arg1;
		m_mouseY = e.arg2;
		//SDL_Log("%d %d", m_mouseX, m_mouseY);

		if (m_touch.touching) {
			m_touch.dx = e.arg1 - m_touch.x;
			m_touch.dy = e.arg2 - m_touch.y;
			m_touch.cur_x = e.arg1;
			m_touch.cur_y = e.arg2;
		}
	}
}

void InputHolder::Update(float deltaTime)
{
	if (m_controller != NULL)
		m_controller->Control(*this,deltaTime);
}
