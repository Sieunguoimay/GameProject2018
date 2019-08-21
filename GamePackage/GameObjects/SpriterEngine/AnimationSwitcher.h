#pragma once
#include"Animation.h"
class AnimationSwitcher :public AnimationBase{
protected:
	int m_nextAnimationIndex;
	std::vector<TimelineKey*>pSliceA;
	std::vector<TimelineKey*>*pSliceB;

	//extra pointer
	void*m_extraPointer;
	std::vector<BodySegment*>*m_pControlKeys;
public:
	//called one time
	AnimationSwitcher(ScmlObject*pOwner, ScmlEntity*pEntity,void*extraPointer, std::vector<BodySegment*>*pControlKeys);
	virtual ~AnimationSwitcher() override;

	//called many times
	virtual void Init(AnimationBase*animationA,AnimationBase*animationB, 
				int switchingTime,int nextAnimationIndex);
	virtual float UpdateAndDraw(float newTime) override;
	void*GetOwner() { return m_pOwner; }
	int GetNextAnimationIndex() { return m_nextAnimationIndex; }
};
