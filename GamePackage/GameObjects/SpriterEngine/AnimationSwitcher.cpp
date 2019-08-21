#include "AnimationSwitcher.h"
#include"../../misc/Math/Geometry.h"
#include"ScmlEntity.h"
#include"ScmlObject.h"
AnimationSwitcher::AnimationSwitcher(ScmlObject*pOwner, ScmlEntity*pEntity, void * extraPointer, 
	std::vector<BodySegment*>*pControlKeys)
	:AnimationBase(pOwner,pEntity)
	, m_extraPointer(extraPointer)
	, m_pControlKeys(pControlKeys)
{
}

AnimationSwitcher::~AnimationSwitcher()
{
	for (auto&a : pSliceA)
		delete a;
}

void AnimationSwitcher::Init( AnimationBase*animationA, AnimationBase*animationB, int switchingTime, int nextAnimationIndex)
{
	//keep the sliceA first
	bool firstTime = timelineSlice.empty();
	std::vector<TimelineKey*>&sliceA = animationA->GetSlice();
	for (int i = 0; i < sliceA.size(); i++) {
		//just fill it up for the fist time
		if (firstTime) {
			//for this timelineSlice
			timelineSlice.push_back(sliceA[i]->Spawn());

			//for the sliceA to keep track
			pSliceA.push_back(sliceA[i]->Spawn());
			sliceA[i]->Clone(pSliceA[i]);
			pSliceA[i]->time = 0;
		}
		else {//from the second time on
			//just replace the pSliceA's elements.
			sliceA[i]->Clone(pSliceA[i]);
			pSliceA[i]->time = 0;
		}
	}
	mainKey = animationA->mainKey;

	//then sliceB
	float max_angle_diff = ((Animation*)animationB)->CalculateSliceAtZero(&pSliceA);
	pSliceB = &animationB->GetSlice();


	static const float angle_speed = 90.0f / 300.0f;
	if (switchingTime == -1)
		length = (int)glm::max((max_angle_diff / angle_speed),10.0f);
	else
		length = switchingTime;

	m_nextAnimationIndex = nextAnimationIndex;
	m_done = false;
}
float AnimationSwitcher::UpdateAndDraw(float newTime)
{
	//what going on in the swicthingTime interval goes here..

	if (mainKey == NULL) return 0;
	newTime = glm::min(newTime, (float)length);

	int boneNum = mainKey->boneRefs.size();
	int objectNum = mainKey->objectRefs.size();

	for (int b = 0; b < boneNum; b++)
	{
		Ref currentRef = mainKey->boneRefs[b];
		BoneTimelineKey*currentKey = (BoneTimelineKey*)timelineSlice[b];// new BoneTimelineKey();

		pSliceA.at(b)->Interpolate(currentKey, pSliceB->at(b),length, newTime);
		float angleA = ((BoneTimelineKey*)pSliceA.at(b))->info.angle;
		float angleB = ((BoneTimelineKey*)pSliceB->at(b))->info.angle;
		if (std::abs(angleB - angleA)>180.0f) {
			if (angleA > angleB) angleA -= 360.0f;
			else angleA += 360.0f;
		}
		currentKey->info.angle = _linear(angleA, angleB, newTime / (float)length);
		m_pOwner->callback->UpdateBoneFromKey(b, (BoneTimelineKey*)timelineSlice[b]);
	}

	for (int o = 0; o < objectNum; o++)
	{
		Ref currentRef = mainKey->objectRefs[o];
		pSliceA.at(boneNum + o)->Interpolate(timelineSlice[boneNum + o], pSliceB->at(boneNum + o), length, newTime);
		m_pOwner->callback->UpdateSpriteFromKey(o, (SpriteTimelineKey*)timelineSlice[boneNum + o]);
	}


	if (newTime == length) {
		m_done = true;
		SDL_Log("done");
		return 0;
	}
	return newTime;
}






