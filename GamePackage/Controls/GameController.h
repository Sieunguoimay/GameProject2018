#pragma once
#include"InputHolder.h"
#include"../GameObjects/MovingObject.h"
//this class is used to controll a specific 
//moving object by process the touch event
//and key event for windows
class GameController {
	MovingObject*m_pSubject;
	b2Vec2 m_diff;
	b2Vec2 m_dir;
	float m_boundRadius;
	bool m_pressing;
	bool m_subjectInDirection;

	Sprite*m_pSpriteBase;
	Sprite*m_pSpriteButton;

	b2Vec2 m_pos1;
	b2Vec2 m_pos0;

	float m_delta;
public:
	GameController(Sprite*spriteBase, Sprite*spriteButton,b2Vec2 pos);
	~GameController();

	void Draw();//display
	void Update(float deltaTime);

	void HandleEvent(const Key&key);
	void HandleEvent(const Touch&touch);
	void SetSubject(MovingObject*subject);
	void SetBoundRadius(float radius) { this->m_boundRadius = radius; }
};
