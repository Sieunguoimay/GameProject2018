#pragma once
#include"Entity.h"
#include"SpriterEngine\ScmlObject.h"

class SpriterEntity:public Entity {	
	int m_animationIndex = 0;
	int m_currentTime = 0;

	//index keepers
	int m_scmlEntityIndex;
	int m_animationSwitcherIndex;

	//Pointers
	ScmlObject*m_pScmlObject;//this object keep the characterInfo tobe changed
	std::map<std::string, int>* m_animationMap;
	std::vector<AnimationBase*>*m_animationContainer;

	//keepers
	//FlipType m_flip = FLIP_NONE;
public:

	SpriterEntity(ScmlObject*pScmlObject, int scmlEntityIndex);
	~SpriterEntity();
	SpriterEntity*Spawn();

	void SetTimeToZero();
	void SetAnimation(std::string name);
	int GetAnimationIndex(std::string name);
	bool HasDone();//passing the signal for noLooping animation

	void Update(float deltaTime);
	void Draw();//for testing purpose

	void Draw(glm::vec2 pos, float angle, float scaleX, float scaleY,FlipType flip = FLIP_NONE);

	void Draw(glm::vec2 pos, float angle, float scale);
	void Draw(glm::vec4 destRect, float angle);

	ScmlEntity*GetSclmEntity();
};