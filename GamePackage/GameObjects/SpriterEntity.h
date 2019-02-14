#pragma once
#include"Entity.h"
#include"SpriterEngine\ScmlObject.h"

class SpriterEntity:public Entity {	
	int m_actualAnimationIndex = 0;
	int m_animationIndex = 0;
	int m_currentTime = 0;

	//index keepers
	int m_scmlEntityIndex;
	int m_animationSwitcherIndex;

	//Pointers
	ScmlObject*m_pScmlObject;//this object keep the characterInfo tobe changed
	std::map<std::string, int>* m_animationMap;
	std::vector<AnimationBase*>*m_animationContainer;

public:

	SpriterEntity(ScmlObject*pScmlObject, int scmlEntityIndex);
	~SpriterEntity();
	SpriterEntity*Spawn();

	void Update(float deltaTime);

	void Draw()override;//for testing purpose
	void Draw(glm::vec2 pos, float angle, float scaleX, float scaleY,FlipType flip = FLIP_NONE);
	void Draw(glm::vec2 pos, float angle, float scale);
	void Draw(glm::vec4 destRect, float angle);


	void TakeControl(Timeline*timeline);
	void ReleaseControl(Timeline*timeline);
	void ControlAngle(Timeline*timeline, float angle);

	ScmlEntity*GetSclmEntity();
	int GetDefaultTimelineIndex(const char*timelineName);
	Timeline*GetTimeline(const char*animationName, const char*timelineName);
	Timeline*GetTimeline(int animationIndex, const char*timelineName);
	Timeline*GetTimeline(int animationIndex, int timelineIndex);

	void SetTimeToZero();
	void SetAnimation(const char* name);
	void SetAnimation(int newAnimationIndex);

	int GetAnimationIndex(const char* name);
	inline const int& GetCurrentAnimationIndex()const { return m_animationIndex; }
	inline const int& GetActualAnimationIndex()const { return m_actualAnimationIndex; }
	bool HasDone();//passing the signal for noLooping animation
};