#pragma once
#include"EntityHierachy/Entity.h"
#include"SpriterEngine\ScmlObject.h"
#include"SpriterEngine\BodySegment.h"

class SpriterEntity :public AnimationCallBack {
	ScmlObject*m_pScmlObject;//this object keep the characterInfo tobe changed




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
public:

	SpriterEntity(ScmlObject*pScmlObject, int scmlEntityIndex);//default
	SpriterEntity(SpriterEntity*spriterEntity);//copy
	~SpriterEntity();
	SpriterEntity*Spawn();

	void Update(float deltaTime);
	void Draw(glm::vec2 pos, float angle, float scaleX, float scaleY, FlipType flip = FLIP_NONE);

	BodySegment*GetBodySegment(int boneIndex);
	float GetAngleOfBone(int boneIndex);
	int GetSegmentIndex(const char*timelineName);


	//the root info
	SpatialInfo characterInfo;
	glm::vec4 m_AABB;
	//equal to number of timelines
	//callback functions
	void UpdateBoneFromKey(int boneIndex, BoneTimelineKey*boneKey, const FlipType&tempFlip)override;
	void UpdateSpriteFromKey(int spriteIndex, SpriteTimelineKey*spriteKey, const FlipType&tempFlip)override;



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
