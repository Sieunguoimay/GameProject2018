#pragma once
#include"Sprite.h"
#include"Animation.h"
class AnimatedSprite {
	Animation*m_pAnimation;
	Sprite*m_pSprite;

	int m_currentSprite;
	float m_timer;
public:
	AnimatedSprite(Sprite*m_sprite);
	~AnimatedSprite();


	void Update(float deltaTime);
	void Draw(float x, float y, float w, float h, float angle = 0.0f, float alpha = 1.0f,float z = 0.0f, int flip = Sprite::FLIP_DEFAULT);


	void SetAnimation(std::string name);//set to no_animation if no demand
	int GetCurrentSprite() { return this->m_currentSprite; }
	Animation*GetAnimation() { this->m_pAnimation; }
	Sprite*GetSprite() { return this->m_pSprite; }
};
