#pragma once
#include<map>
#include"Timeline.h"
#include"MainlineKey.h"
#include"Folder.h"
class ScmlEntity;
class ScmlObject;
//TimelineSlice: to serve for animation switching
//having the ability of keeping the current slad of 
//timeline of the current animation. so that when
//the switcher requires a slice. both current animation
//and the switcher itself have the ability to give out
//its slice.

class AnimationCallBack {
public:
	virtual void UpdateBoneFromKey(int boneIndex, BoneTimelineKey*boneKey) = 0;
	virtual void UpdateSpriteFromKey(int spriteIndex, SpriteTimelineKey*spriteKey) = 0;
};

class AnimationBase {
protected:
	std::vector<TimelineKey*>timelineSlice;
	bool m_done;//signaling out used for no loop animation
	ScmlEntity*m_pEntity;//to serve for callback on taking out slice of keys
	ScmlObject*m_pOwner;
public:
	AnimationBase(ScmlObject*owner, ScmlEntity*pEntity);
	virtual ~AnimationBase();
	virtual float UpdateAndDraw(float newTime)=0;
	virtual std::vector<TimelineKey*>&GetSlice(){return timelineSlice;}
	bool HasDone() { return m_done; }
	int length;
	MainlineKey*mainKey;//this fucking key is not belong to the scml project
	//TimelineKey* KeyFromRef(Ref ref, int newTime);
};



class Animation :public AnimationBase {
	enum AnimationLoopType { NO_LOOPING, LOOPING };
	Animation*m_pFirstAnimation;
public:
	std::string name;
	int loopType = LOOPING;
	std::vector<MainlineKey*>mainlineKeys;
	std::vector<Timeline*>timelines;


	Animation(ScmlObject*pOwner, ScmlEntity*pEntity);
	~Animation() override;
	void Log();
	MainlineKey* MainlineKeyFromTime(int time);
	void KeyFromRef(TimelineKey*pKey, Ref ref, int newTime, bool isBone = false);
	float CalculateSliceAtZero(const std::vector<TimelineKey*>*sliceA);

	//public used.
	float UpdateAndDraw(float newTime) override;
	void UpdateCharacter(MainlineKey*mainKey, int newTime);

};
