#pragma once
#include<map>
#include"Timeline.h"
#include"MainlineKey.h"
#include"Folder.h"
//TimelineSlice: to serve for animation switching
//having the ability of keeping the current slad of 
//timeline of the current animation. so that when
//the switcher requires a slice. both current animation
//and the switcher itself have the ability to give out
//its slice.

class AnimationCallBack {
public:
	virtual ~AnimationCallBack() {}
	virtual void UpdateBoneFromKey(int boneIndex, BoneTimelineKey*boneKey) = 0;
	virtual void UpdateSpriteFromKey(int spriteIndex, SpriteTimelineKey*spriteKey) = 0;
	virtual TimelineKey*GetTimelineKeyFromSlice(int index) = 0;
};

class AnimationBase {
protected:
	bool m_done;//signaling out used for no loop animation
public:
	AnimationBase();
	virtual ~AnimationBase();
	virtual float Update(float newTime, int*currentMainlineKeyIndex, AnimationCallBack*callback)=0;
	virtual void Draw(int newTime, AnimationCallBack*callback)=0;
	bool HasDone() { return m_done; }
	int length;
	MainlineKey*mainKey;
};



class Animation :public AnimationBase {
	enum AnimationLoopType { NO_LOOPING, LOOPING };
	Animation*m_pFirstAnimation;
public:
	std::string name;
	int loopType = LOOPING;
	std::vector<MainlineKey*>mainlineKeys;
	std::vector<Timeline*>timelines;


	~Animation() override;
	void Log();
	MainlineKey* MainlineKeyFromTime(int time, int*currentMainlineKeyIndex);
	void KeyFromRef(TimelineKey*pKey, Ref ref, int newTime, bool isBone = false);
	float CalculateSliceAtZero(std::vector<TimelineKey*>&timelineSlice,const std::vector<TimelineKey*>*sliceA);

	//public used.
	float Update(float newTime, int*currentMainlineKeyIndex, AnimationCallBack*callback) override;
	void Draw(int newTime, AnimationCallBack*callback)override;

};
