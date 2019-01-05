#include "SpriterEntity.h"
#include<algorithm>
#include"SpriterEngine\AnimationSwitcher.h"
SpriterEntity::SpriterEntity(ScmlObject*pScmlObject, int scmlEntityIndex)
	:m_pScmlObject(pScmlObject),m_scmlEntityIndex(scmlEntityIndex)
{
	//once for all
	m_animationMap = &(m_pScmlObject->entities[m_scmlEntityIndex]->animationMap);
	m_animationContainer = &(m_pScmlObject->entities[m_scmlEntityIndex]->animations);

	//each spriterEntity has an animationSwitcher only for using multiple times. 
	std::vector<AnimationBase*>*animationContainer = &(m_pScmlObject->entities[m_scmlEntityIndex]->animations);
	animationContainer->push_back(new AnimationSwitcher(&m_pScmlObject->characterInfo,(void*)this));
	m_animationSwitcherIndex = animationContainer->size()-1;
	m_animationIndex = -1;
}
SpriterEntity::~SpriterEntity() {
	//erasing what you messed up at the beginning

	//get the temp animation for swapping

	//correct the index going tobe kept in the last element while jumping to the new location in the vector
	((SpriterEntity*)(((AnimationSwitcher*)(*(m_animationContainer->end() - 1)))->GetOwner()))->m_animationSwitcherIndex = m_animationSwitcherIndex;

	//swap
	std::iter_swap(m_animationContainer->begin() + m_animationSwitcherIndex, m_animationContainer->end()-1);

	//destroy the last one
	delete *(m_animationContainer->end() - 1);//delete the dynamic memory
	m_animationContainer->erase(m_animationContainer->end() - 1);//remove the last element
}



#include"../misc/Locator.h"
void SpriterEntity::Update(float deltaTime)
{
	m_currentTime += (int)(deltaTime*1000.0f);
}

void SpriterEntity::Draw()
{
	Draw(glm::vec2(0, 0), 0.0f, 1.0f);
}
bool SpriterEntity::HasDone() {
	if (m_animationIndex == -1)
		return false;

	if (m_animationIndex != m_animationSwitcherIndex)
		return m_animationContainer->at(m_animationIndex)->HasDone();
	
	return false;
}
void SpriterEntity::Draw(glm::vec2 pos, float angle, float scaleX, float scaleY, FlipType flip)
{
	if (m_animationIndex == -1) return;
	if (m_animationIndex == m_animationSwitcherIndex) {
		AnimationSwitcher*temp = (AnimationSwitcher*)(m_animationContainer->at(m_animationIndex));
		if (temp->HasDone())//gets the out going signal
			m_animationIndex = temp->GetNextAnimationIndex();
	}

	m_pScmlObject->characterInfo.x = pos.x;
	m_pScmlObject->characterInfo.y = pos.y;
	m_pScmlObject->characterInfo.angle = angle;
	m_pScmlObject->characterInfo.scaleX = scaleX;
	m_pScmlObject->characterInfo.scaleY = scaleY;

	m_pScmlObject->currentEntity = m_scmlEntityIndex;
	m_pScmlObject->currentAnimation = m_animationIndex;
	m_currentTime = m_pScmlObject->Draw(m_currentTime, flip);
}

void SpriterEntity::Draw(glm::vec2 pos, float angle, float scale)
{
	Draw(pos, angle, scale, scale,FlipType::FLIP_NONE);
}

void SpriterEntity::Draw(glm::vec4 destRect, float angle)
{
	float scaleX = 1;
	float scaleY = 1;
	Draw(destRect, angle,scaleX,scaleY, FlipType::FLIP_NONE);
}

ScmlEntity * SpriterEntity::GetSclmEntity()
{
	return m_pScmlObject->entities[m_scmlEntityIndex];
}


SpriterEntity * SpriterEntity::Spawn()
{
	return new SpriterEntity(m_pScmlObject,m_scmlEntityIndex);
}

void SpriterEntity::SetTimeToZero()
{
	m_currentTime = 0;
}

void SpriterEntity::SetAnimation(std::string name)
{
	m_currentTime = 0;
	auto animation = m_animationMap->find(name);
	if (animation == m_animationMap->end()) {
		SDL_Log("Animation %s name Not found. Set to default (0)", name.c_str());
		m_animationIndex = 0;
		return;
	}


	// in case of the first time
	if (m_animationIndex == -1) {
		m_animationIndex = animation->second;
		//just fill the of the first animation and do not care about the return value
		((Animation*)m_animationContainer->at(m_animationIndex))->CalculateSliceAtZero(NULL);
		return;
	}


	//take out the switching times from the container at the positions
	//of valid index of animations which are less than the size of 
	//animation container
	int switchingTime = -1;
	if(m_animationIndex<m_animationMap->size()&& animation->second<m_animationMap->size())
		switchingTime  = (m_pScmlObject->entities[m_scmlEntityIndex]
						->animationSwitchingTime).at(m_animationIndex).at(animation->second);
	//SDL_Log("Switching Time: %d ", switchingTime);
	//take out the animationSwitcher of this spriterEntity and init() it again
	//with the new pair of animations
	((AnimationSwitcher*)(m_animationContainer->at(m_animationSwitcherIndex)))->Init(
		//(animationContainer->at(m_animationIndex)->mainKey),//get the mainKey of the first animation
		m_animationContainer->at(m_animationIndex),//this animation maybe a switcher or an Animation Object.
		m_animationContainer->at(animation->second),//the second animation here is properly a real Animation Object
		switchingTime,animation->second);

	m_animationIndex = m_animationSwitcherIndex;

}

int SpriterEntity::GetAnimationIndex(std::string name)
{
	auto animation = m_animationMap->find(name);
	if (animation == m_animationMap->end()) {
		SDL_Log("Animation %s name Not found. Set to default (0)", name.c_str());
		return -1;
	}
	return animation->second;
}
