#include"AnimatedSprite.h"
#include"SpriteManager.h"
AnimatedSprite::AnimatedSprite(Sprite*sprite)
	:m_pSprite(sprite)
	,m_timer(0.0f)
	,m_currentSprite(0)
	,m_pAnimation(NULL)
{
}
AnimatedSprite::~AnimatedSprite() {
	m_pSprite = NULL;
}
void AnimatedSprite::Update(float deltaTime){
	if (m_pAnimation != NULL) {
		m_timer += deltaTime;
		if (m_timer >= m_pAnimation->GetSPF()) {
			m_timer = 0.0f;
			m_currentSprite++;
			if (m_currentSprite >= m_pAnimation->GetSpriteNum())
				m_currentSprite = 0;
		}
	}
}
void AnimatedSprite::Draw(float x, float y, float w, float h, float angle ,float alpha, float z , int flip ) {
	glm::vec2 index(0.0f,0.0f);
	if (m_pAnimation != NULL) {
		m_pAnimation->SetSprite(m_currentSprite);
		index = m_pAnimation->GetIndex();
	}
	m_pSprite->Draw(index.x, index.y, x, y, w, h, angle, alpha, z, flip);
}

void AnimatedSprite::SetAnimation(std::string name)
{
	auto&animationSet = *s_spriteManager.GetAnimationSet(m_pSprite);
	if (animationSet.size() != 0) {
		auto animation = animationSet.find(name);
		if (animation != animationSet.end()) {
			m_pAnimation = animation->second;
			return;
		}
	}
	m_pAnimation = NULL;
}
