#pragma once
#include"EntityHierachy/Entity.h"
#include"SpriterEngine\ScmlObject.h"
#include"SpriterEngine\BodySegment.h"

class SpriterEntity :public AnimationCallBack {
	friend class AssetsManager;
	SpriterEntity*Spawn();
	
	//all this stuff is to deal with the problem of animation switching
	int m_currentMainlineKeyIndex = 0;//used to fast access next mainlinekey from time
	int m_animationSwitcherIndex = 0;
	int m_actualAnimationIndex = -1;
	int m_animationIndex = -1;
	int m_currentTime = 0;
	bool m_aabbFlag = true;
	bool m_updatePositionFlag = true;

	//Pointers to something that is unique for many spriter entity
	std::vector<SpriterEngine::Folder*>*m_pFolders;
	ScmlEntity*m_pScmlEntity;

	BoneTimelineKey m_tempKey;
	std::vector<TimelineKey*>m_timelineSlice;//temporary
	//control bones
	std::vector<BoneSegment*>m_boneSegments;//unmapped from parent
	std::vector<SpriteSegment*>m_spriteSegments;
	//the root info
	SpatialInfo characterInfo;
	glm::vec4 m_AABB;
	glm::vec4 m_defaultAABB;
	glm::vec2 m_defaultOriginInAABB;
	FlipType m_flip;

public:

	SpriterEntity(ScmlEntity*entity, std::vector<SpriterEngine::Folder*>*m_pFolders);//default
	SpriterEntity(SpriterEntity*spriterEntity, bool calculateAABB = false);//copy
	~SpriterEntity()override;

	void Update(float deltaTime);
	void Draw();

	//callback functions
	void UpdateBoneFromKey(int boneIndex, BoneTimelineKey*boneKey)override;
	void UpdateSpriteFromKey(int spriteIndex, SpriteTimelineKey*spriteKey)override;
	TimelineKey*GetTimelineKeyFromSlice(int index) override;

	//passing the signal for noLooping animation
	bool HasDone();

	void SetAnimationSwitchingTime(const char*animationA, const char*animationB, int time);
	void SetAnimation(const char* name);
	void SetAnimation(int newAnimationIndex, glm::vec2 jumpDistance = glm::vec2(0,0));
	//dont use this function at all. this is to serve for the Editor only.
	void SetAnimationStrictly(int index)			{ m_animationIndex = index; m_currentTime = 0; }
	void SetAngle(float angle)						{ characterInfo.angle = angle; }
	void SetPos(const glm::vec2&pos);
	void SetAABB(const glm::vec4&AABB);
	void SetFlip(const FlipType&flip)				{ m_flip = flip; }
	void SetUpdatePositionFlag(bool flag)			{ m_updatePositionFlag = flag; }

	int GetCurrentAnimationIndex()const				{ return m_animationIndex; }
	int GetActualAnimationIndex()const				{ return m_actualAnimationIndex; }
	int GetAnimationIndex(const char* name);
	int GetSegmentIndex(const char*timelineName);
	BoneSegment*GetBodySegment(int boneIndex);
	const glm::vec4& GetAABB() const				{ return m_AABB; }
	const FlipType& GetFlip() const					{ return m_flip; }
	const SpatialInfo&GetSpatialInfo()const			{ return characterInfo; }
	friend class AnimationSwitcher;
};
