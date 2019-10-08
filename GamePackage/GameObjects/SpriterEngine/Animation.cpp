#include "Animation.h"
#include"ScmlEntity.h"
#include"ScmlObject.h"
AnimationBase::AnimationBase()
	:mainKey(NULL),
	m_done(false)
{
}
AnimationBase::~AnimationBase() {
	//cleanup temp vector if any left
}


Animation::~Animation()
{
	for (auto&timeline : timelines)
		delete timeline;
	for (auto&mainlineKey : mainlineKeys)
		delete mainlineKey;
	printf("Deleted Animation\n");

}

void Animation::Log()
{
	SDL_Log("\tanimation %s %d %d\n", name.c_str(), length, loopType);

	for (auto&mainlineKey : mainlineKeys)
		mainlineKey->Log();
	for (auto&timeline : timelines)
		timeline->Log();
}

//this function is important for the first time only
//in case this animation has not been executed yet.
//but the timeline slice is must be already there ready for usage. 
float Animation::CalculateSliceAtZero(std::vector<TimelineKey*>&timelineSlice, const std::vector<TimelineKey*>*sliceA) {
	m_done = false;

	int newTime = 0;
	mainKey = MainlineKeyFromTime(newTime,NULL);//keep a copy of pointer
	
	float max_angle_difference = 0;
	int boneNum = mainKey->boneRefs.size();
	int objectNum = mainKey->objectRefs.size();
	bool firstTime = timelineSlice.empty();
	if (firstTime) timelineSlice.reserve(boneNum + objectNum);

	for (int b = 0; b < boneNum; b++)
	{
		if (firstTime)
			timelineSlice.push_back(new BoneTimelineKey());

		//for the fist time the prev slice will be empty
		if (sliceA == NULL)continue;

		//create the new timelineKey to fill up the timelineSlice
		TimelineKey*currentKey = timelineSlice[b];// new BoneTimelineKey();
		KeyFromRef(currentKey, mainKey->boneRefs[b], newTime);

		//calculate the angle difference here.
		float angleA = ((BoneTimelineKey*)sliceA->at(b))->m_info.angle;
		float angleB = ((BoneTimelineKey*)currentKey)->m_info.angle;

		if (std::abs(angleB - angleA)>180.0f) {
			if (angleA > angleB) angleA -= 360.0f;
			else angleA += 360.0f;
		}
		float angle_difference = std::abs(angleB - angleA);
		if (max_angle_difference < angle_difference)
			max_angle_difference = angle_difference;
	}

	for (int o = 0; o < objectNum; o++) {

		if (firstTime)
			timelineSlice.push_back(new SpriteTimelineKey());

		//for the fist time the prev slice will be empty
		if (sliceA == NULL)continue;

		//create the new timelineKey to fill up the timelineSlice
		TimelineKey*currentKey = timelineSlice[boneNum + o];// new SpriteTimelineKey();
		KeyFromRef(currentKey, mainKey->objectRefs[o], newTime);

		//calculate the angle difference here.
		float angleA = ((SpriteTimelineKey*)sliceA->at(boneNum+o))->m_info.angle;
		float angleB = ((SpriteTimelineKey*)currentKey)->m_info.angle;
		if (std::abs(angleB - angleA)>180.0f) {
			if (angleA > angleB) angleA -= 360.0f;
			else angleA += 360.0f;
		}
		float angle_difference = std::abs(angleB - angleA);
		if (max_angle_difference < angle_difference)
			max_angle_difference = angle_difference;
	}

	return max_angle_difference;
}






float Animation::Update(float newTime, int*currentMainlineKeyIndex, AnimationCallBack*callback)
{
	if (loopType == NO_LOOPING)
	{
		newTime = glm::min(newTime, (float)length);
		if (newTime == length)
			m_done = true;//signal out
	}
	else if (loopType == LOOPING)
	{
		newTime = ((int)newTime) % length;
	}
	mainKey = MainlineKeyFromTime(newTime, currentMainlineKeyIndex);//keep a copy of pointer

	int boneNum = mainKey->boneRefs.size();
	std::vector<BoneTimelineKey> transformBoneKeys(boneNum);
	for (int b = 0; b < boneNum; b++)
	{
		Ref currentRef = mainKey->boneRefs[b];
		//auto boneKey = timelineSlice[b];
		//KeyFromRef(boneKey, currentRef, newTime);
		if (callback != NULL) {
			auto boneKey = callback->GetTimelineKeyFromSlice(b);
			KeyFromRef(boneKey, currentRef, newTime);
			callback->UpdateBoneFromKey(b, ((BoneTimelineKey*)boneKey));
		}
	}

	return newTime;
}

void Animation::Draw(int newTime, AnimationCallBack*callback)
{
	if (callback == NULL)return;

	int boneNum = mainKey->boneRefs.size();
	int objectNum = mainKey->objectRefs.size();
	for (int o = 0; o < objectNum; o++)
	{
		Ref currentRef = mainKey->objectRefs[o];
		auto spriteKey = callback->GetTimelineKeyFromSlice(boneNum+o);
		KeyFromRef(spriteKey, currentRef, newTime);
		callback->UpdateSpriteFromKey(o, (SpriteTimelineKey*)spriteKey);
	}
	// <expose objectKeys to api users to retrieve AND replace objectKeys>

}

MainlineKey * Animation::MainlineKeyFromTime(int currentTime, int*currentMainlineKeyIndex)
{
	if (currentMainlineKeyIndex != NULL) {
		if (mainlineKeys[*currentMainlineKeyIndex]->time <= currentTime) {
			while (*currentMainlineKeyIndex + 1< mainlineKeys.size()&&
				mainlineKeys[*currentMainlineKeyIndex+1]->time < currentTime) 
					(*currentMainlineKeyIndex)++;
			
			return mainlineKeys[*currentMainlineKeyIndex];
		}
	}


	int currentMainKey = 0;
	for (int m = 0, n=mainlineKeys.size(); m < n; m++)
	{
		if (mainlineKeys[m]->time <= currentTime)
			currentMainKey = m;
		if (mainlineKeys[m]->time >= currentTime)
			break;
	}
	if (currentMainlineKeyIndex != NULL)
		*currentMainlineKeyIndex = currentMainKey;
	return mainlineKeys[currentMainKey];
}

void Animation::KeyFromRef(TimelineKey*pKey, Ref ref, int newTime, bool isBone /*= false*/)
{



	Timeline*timeline = timelines.at(ref.timeline);

	//if this timeline is under control, then give the info of keys[0] to the target key
	//no need for any interpolate()

	TimelineKey*keyA = timeline->keys[ref.key];

	//timeline has only one key
	if (timeline->keys.size() == 1)
	{
		keyA->Clone(pKey);
		return;
	}

	//for the last key
	int nextKeyIndex = ref.key + 1;
	if (nextKeyIndex >= timeline->keys.size())
	{
		if (loopType == LOOPING)
		{
			nextKeyIndex = 0;
		}
		else
		{
			keyA->Clone(pKey);
			return;
		}
	}

	TimelineKey*keyB = timeline->keys[nextKeyIndex];
	

	int keyBTime = keyB->time;

	if (keyBTime < keyA->time)
	{
		keyBTime = keyBTime + length;
	}
	//SDL_Log("Heyzo %d %d %d",nextKeyIndex, keyBTime, keyA->time);

	keyA->Interpolate(pKey, keyB, keyBTime, newTime);
}
