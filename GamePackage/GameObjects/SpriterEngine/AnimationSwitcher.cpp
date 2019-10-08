#include "AnimationSwitcher.h"
#include"../../misc/Math/Geometry.h"
#include"ScmlEntity.h"
#include"ScmlObject.h"
#include"../SpriterEntity.h"
#include"../../misc/Assistances.h"
AnimationSwitcher::~AnimationSwitcher()
{
	for (auto&a : m_sliceA)delete a;
	for (auto&a : m_sliceB)delete a;
}
void AnimationSwitcher::Init( AnimationBase*animationA, AnimationBase*animationB, int switchingTime, int nextAnimationIndex, const glm::vec2&jumpDistance)
{
	//m_pRootBoneKey = NULL;
	m_jumpDistance = jumpDistance;
	if (m_jumpDistance.x != 0 || m_jumpDistance.y != 0) {
		m_appliedJumpDistance = false;
	}
	//keep the sliceA first
	bool firstTime = m_sliceA.empty();// timelineSlice.empty();
	std::vector<TimelineKey*>&sliceA = m_pOwner->m_timelineSlice;// animationA->GetSlice();

	int boneNum = animationA->mainKey->boneRefs.size();
	int spriteNum = animationA->mainKey->objectRefs.size();

	if (firstTime) 
		m_sliceA.reserve(boneNum + spriteNum);

	for (int i = 0; i < sliceA.size(); i++) {
		//just fill it up for the fist time
		if (firstTime) 
			m_sliceA.push_back(sliceA[i]->Spawn());
		
		sliceA[i]->Clone(m_sliceA[i]);
		m_sliceA[i]->time = 0;

		if (i < boneNum) {
			auto currentRef = animationA->mainKey->boneRefs[i];
			if (currentRef.parent == -1 && !m_appliedJumpDistance) {

				if(m_pOwner->GetFlip()==HORIZONTAL_FLIP)
					m_jumpDistance.x = -m_jumpDistance.x;//???
		
				auto m_pRootBoneKey = (BoneTimelineKey*)m_sliceA[i];
				m_pRootBoneKey->m_info.x = m_pRootBoneKey->m_info.x - m_jumpDistance.x;
				m_pRootBoneKey->m_info.y = m_pRootBoneKey->m_info.y - m_jumpDistance.y;
				m_appliedJumpDistance = true;
			}
		}
	}
	mainKey = animationA->mainKey;

	//then sliceB
	float max_angle_diff = ((Animation*)animationB)->CalculateSliceAtZero(m_sliceB,&m_sliceA);
	//pSliceB = &animationB->GetSlice();


	static const float angle_speed = 90.0f / 300.0f;
	if (switchingTime == -1) length = (int)glm::max((max_angle_diff / angle_speed),10.0f);
	else length = switchingTime;

	m_nextAnimationIndex = nextAnimationIndex;
	m_done = false;
}

float AnimationSwitcher::Update(float newTime, int*currentMainLineKeyIndex,AnimationCallBack*callback)
{
	//what going on in the swicthingTime interval goes here..

	if (mainKey == NULL) return 0;
	newTime = glm::min(newTime, (float)length);

	int boneNum = mainKey->boneRefs.size();

	for (int b = 0; b < boneNum; b++)
	{
		Ref currentRef = mainKey->boneRefs[b];
		if (callback != NULL){

			BoneTimelineKey*currentKey = (BoneTimelineKey*)callback->GetTimelineKeyFromSlice(b);// new BoneTimelineKey();
			BoneTimelineKey*prevKey = (BoneTimelineKey*)m_sliceA[b];// (BoneTimelineKey*)callback->GetTimelineKeyFromSliceA(b);
			BoneTimelineKey*nextKey = (BoneTimelineKey*)m_sliceB[b];// (BoneTimelineKey*)callback->GetTimelineKeyFromSliceB(b);

			currentKey->m_info = SpatialInfo::linear(prevKey->m_info, nextKey->m_info, 1, prevKey->GetTWithNextKey(length, newTime));

			float angleA = prevKey->m_info.angle;
			float angleB = nextKey->m_info.angle;
			if (std::abs(angleB - angleA) > 180.0f) {
				if (angleA > angleB) angleA -= 360.0f;
				else angleA += 360.0f;
			}
			currentKey->m_info.angle = _linear(angleA, angleB, newTime / (float)length);
			callback->UpdateBoneFromKey(b, currentKey);
		}
	}

	

	if (newTime == length) {
		m_done = true;
		SDL_Log("done");
		return 0;
	}
	return newTime;
}

void AnimationSwitcher::Draw(int newTime, AnimationCallBack * callback)
{
	if (callback == NULL)return;
	int boneNum = mainKey->boneRefs.size();
	int objectNum = mainKey->objectRefs.size();
	for (int o = 0; o < objectNum; o++)
	{
		Ref currentRef = mainKey->objectRefs[o];
		SpriteTimelineKey* currentKey = (SpriteTimelineKey*)callback->GetTimelineKeyFromSlice(boneNum + o);
		SpriteTimelineKey* prevKey = (SpriteTimelineKey*)m_sliceA[boneNum+o];// callback->GetTimelineKeyFromSliceA(boneNum + o);
		SpriteTimelineKey* nextKey = (SpriteTimelineKey*)m_sliceB[boneNum + o];// callback->GetTimelineKeyFromSliceB(boneNum + o);
		prevKey->Interpolate(currentKey, nextKey, length, newTime);
		callback->UpdateSpriteFromKey(o, currentKey);
	}
}






