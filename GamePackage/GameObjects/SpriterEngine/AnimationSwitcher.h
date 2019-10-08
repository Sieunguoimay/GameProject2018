#pragma once
#include"Animation.h"
class AnimationSwitcher :public AnimationBase{
protected:
	int m_nextAnimationIndex;
	int m_animationSwitcherIndex;
	
	bool m_appliedJumpDistance = true;
	glm::vec2 m_jumpDistance;

	std::vector<TimelineKey*>m_sliceA;
	std::vector<TimelineKey*>m_sliceB;

	//extra pointer
	std::vector<BoneSegment*>*m_pControlKeys;
	class SpriterEntity*m_pOwner;
public:
	//called one time
	AnimationSwitcher(std::vector<BoneSegment*>*pControlKeys, int animationSwitcherIndex, SpriterEntity*pOwner)
		:m_pControlKeys(pControlKeys),m_animationSwitcherIndex(animationSwitcherIndex),m_pOwner(pOwner){}

	virtual ~AnimationSwitcher() override;

	//called many times
	virtual void Init(AnimationBase*animationA,AnimationBase*animationB, int switchingTime,int nextAnimationIndex,const glm::vec2&jumpDistance);
	virtual float Update(float newTime,int*currentMainLineKeyIndex, AnimationCallBack*callback) override;
	virtual void Draw(int newTime, AnimationCallBack*callback)override;

	int GetNextAnimationIndex() { return m_nextAnimationIndex; }
	int GetAnimationSwitcherIndex() { return m_animationSwitcherIndex; }
	void SetAnimationSwitcherIndex(int index) { m_animationSwitcherIndex = index; }
};

