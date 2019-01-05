#include "AnimationCollection.h"
#include<SDL2\SDL.h>

SpriteSheetAnimation::SpriteSheetAnimation(const std::vector<int>& indices, int fps)
	:m_indices(indices),m_fps(fps),m_index(0)
{
}

SpriteSheetAnimation::~SpriteSheetAnimation()
{
}


AnimationCollection::AnimationCollection()
	:m_pCurrentAnimation(NULL),m_time(0.0f)
{
}


AnimationCollection::~AnimationCollection()
{
	for (std::map<std::string, SpriteSheetAnimation*>::const_iterator it = m_animations.begin();
	it != m_animations.end(); ++it) {
		delete it->second;
	}
	delete m_sprite;
}

void AnimationCollection::Init(Texture*texture,glm::ivec2 dimension) {
	m_sprite = new Sprite(texture, dimension);
}
void AnimationCollection::AddAnimation(std::string name, std::vector<int>&indices, int fps) {
	SpriteSheetAnimation*newAnimation = new SpriteSheetAnimation(indices, fps);
	AddAnimation(name, newAnimation);
}

void AnimationCollection::AddAnimation(std::string name, SpriteSheetAnimation * newAnimation){
	m_animations.insert(std::pair<std::string, SpriteSheetAnimation*>(name, newAnimation));
	if (m_pCurrentAnimation == NULL)
		m_pCurrentAnimation = newAnimation;
}

AnimationCollection * AnimationCollection::Clone()
{
	AnimationCollection*newAnimationSystem = new AnimationCollection();
	newAnimationSystem->Init(m_sprite->GetTexture(), m_sprite->GetDimension());
	for (std::map<std::string, SpriteSheetAnimation*>::const_iterator it = m_animations.begin();
		it != m_animations.end(); ++it) {
		newAnimationSystem->AddAnimation(it->first,it->second->GetIndices(),it->second->GetFPS());
	}
	return newAnimationSystem;
}

void AnimationCollection::SwitchAnimation(std::string name)
{
	auto a = m_animations.find(name);
	if (a != m_animations.end()) {
		m_pCurrentAnimation = a->second;
		m_pCurrentAnimation->Reset();
	}
	m_time = 0.0f;
}


void AnimationCollection::Update(float deltaTime) {
	if (m_pCurrentAnimation->GetNumIndices() > 1) {
		m_time += deltaTime;
		if (m_time > 1.0f / (float)m_pCurrentAnimation->GetFPS()) {
			m_pCurrentAnimation->NextFrame();
			m_time = 0.0f;
		}
	}
}
glm::vec4 AnimationCollection::GetCurrentSpriteRect() {
	return m_sprite->GetUVs(m_pCurrentAnimation->GetIndex());
}
GLuint AnimationCollection::GetTextureID() {
	return m_sprite->GetTexture()->GetId();
}
