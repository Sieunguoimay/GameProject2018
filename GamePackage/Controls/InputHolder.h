#pragma once
#include"InputHandler.h"
#include"SDL2\SDL.h"
#include"../2D/Sprite.h"
class InputHolder;
class Controller {
public:
	virtual ~Controller(){}
	virtual void Control(InputHolder&input,float deltaTime) = 0;
};
struct Touch {
	bool touching;
	bool up;
	int x;
	int y;
	int cur_x;
	int cur_y;
	int dx;
	int dy;
	Touch():touching(false),x(-1),y(-1),dx(0),dy(0){}
};
struct Key {
	bool keys[255];
	bool IsPressed(int key) { return keys[key]; }
};

class InputHolder :public InputHandler{
	//InputHolder&operator = (const InputHolder&);
	//InputHolder(const InputHolder&);
	//static InputHolder*s_instance;

	bool m_keyDown;
	bool m_keyUp;
	bool m_keyPressed;

	int m_mouseX;
	int m_mouseY;
	

	bool m_clicked;
	bool m_released;
	bool m_moving;
	
	Touch m_touch;
	Key m_key;

	Controller*m_controller;
public:
	InputHolder();
	~InputHolder();

	void Init(){}
	//static InputHolder&Instance();
	void CleanUp();

	//overidding
	virtual void HandleEvent(Event&e);
	void Update(float deltaTime);



	bool IsKeyDown() { if (m_keyDown) { m_keyDown = false; return true; }return false; }
	bool IsKeyUp() { if (m_keyUp) { m_keyUp = false; return true; }return false; }

	bool isTouching() { if (m_touch.touching) return true; return false; }
	bool IsClicked() { if (m_clicked) { m_clicked = false; return true; }return false; }
	bool IsReleased() { if(m_released){ m_released = false; return true; }return false;}
	bool IsMoving() { if( m_moving){m_moving = false; return true;}return false;}

	int GetMouseX() { return m_mouseX; }
	int GetMouseY() { return m_mouseY; }

	const Touch&GetTouch() const{ return this->m_touch; }
	const Key&GetKey()const { return this->m_key; }

	void SetController(Controller*controller) { this->m_controller = controller; }
	Controller*GetController() { return this->m_controller; }
};
//#define s_inputHolder InputHolder::Instance()