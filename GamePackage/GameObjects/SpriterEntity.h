#pragma once
#include"EntityHierachy/Entity.h"
#include"SpriterEngine\ScmlObject.h"
#include"SpriterEngine\BodySegment.h"

class SpriterEntity :public AnimationCallBack {
	ScmlObject*m_pScmlObject;


	//all this stuff is to deal with the problem of animation switching
	int m_actualAnimationIndex = 0;
	int m_animationIndex = 0;
	int m_currentTime = 0;

	//index keepers
	int m_scmlEntityIndex;
	int m_animationSwitcherIndex;
	int m_actualAnimationNum;


	//Pointers
	std::map<std::string, int>* m_animationMap;
	std::vector<AnimationBase*>*m_animationContainer;

	//control bones
	std::vector<BodySegment*>bodySegments;
	std::vector<SpriteSegment*>spriteSegments;
	//the root info
	SpatialInfo characterInfo;
	glm::vec4 m_AABB;
	FlipType m_flip;
public:

	SpriterEntity(ScmlObject*pScmlObject, int scmlEntityIndex);//default
	SpriterEntity(SpriterEntity*spriterEntity);//copy
	~SpriterEntity();
	SpriterEntity*Spawn();

	void SetInfo(glm::vec2 pos, float angle, float scaleX, float scaleY, FlipType flip = FLIP_NONE);
	void Update(float deltaTime);
	void Draw();

	BodySegment*GetBodySegment(int boneIndex);
	int GetSegmentIndex(const char*timelineName);


	//callback functions
	void UpdateBoneFromKey(int boneIndex, BoneTimelineKey*boneKey)override;
	void UpdateSpriteFromKey(int spriteIndex, SpriteTimelineKey*spriteKey)override;



	void SetAnimationSwitchingTime(const char*animationA, const char*animationB, int time);
	void SetAnimation(const char* name);
	void SetAnimation(int newAnimationIndex);
	
	
	//dont use this function at all. this is to serve for the Editor only.
	void SetAnimationStrictly(int index) { m_animationIndex = index; m_currentTime = 0; }
	int GetAnimationIndex(const char* name);
	inline const int& GetCurrentAnimationIndex()const { return m_animationIndex; }
	inline const int& GetActualAnimationIndex()const { return m_actualAnimationIndex; }

	bool HasDone();//passing the signal for noLooping animation
	
	inline const glm::vec4&GetAABB() { return m_AABB;/* m_animationContainer->at(m_animationIndex)->GetAABB(); */}
};
