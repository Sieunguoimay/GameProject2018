#include"BaseObject.h"

BaseObject::BaseObject(Sprite*sprite,float x, float y, int w, int h) 
	:m_width(w),m_height(h)
	,m_pos(x,y),m_angle(0.0f)
{
	m_animatedSprite = new AnimatedSprite(sprite);
	m_boundRadius = (float)(w + h) / 2.0f;
}
BaseObject::~BaseObject() {
	delete m_animatedSprite;
}




NoPhysicsObject::NoPhysicsObject(Sprite*sprite, float x, float y, int w, int h)
	:BaseObject(sprite,x,y,w,h)
{
}
NoPhysicsObject::~NoPhysicsObject() {
}

void NoPhysicsObject::Update(float deltaTime)
{
	m_animatedSprite->Update(deltaTime);
}

void NoPhysicsObject::Draw()
{
	m_animatedSprite->Draw(m_pos.x, m_pos.y, m_width, m_height, m_angle);
}


