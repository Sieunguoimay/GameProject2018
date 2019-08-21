#include "Animation.h"
#include"ScmlEntity.h"
#include"ScmlObject.h"
AnimationBase::AnimationBase(ScmlObject*owner, ScmlEntity*pEntity)
	:m_pOwner(owner)
	,m_pEntity(pEntity)
	,mainKey(NULL),
	m_done(false)
{
}
AnimationBase::~AnimationBase() {
	//cleanup temp vector if any left
	for (auto&a : timelineSlice)
		delete a;
}


Animation::Animation(ScmlObject*pOwner, ScmlEntity*pEntity)
	:AnimationBase(pOwner,pEntity)
{
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
float Animation::CalculateSliceAtZero(const std::vector<TimelineKey*>*sliceA) {
	m_done = false;

	int newTime = 0;
	mainKey = MainlineKeyFromTime(newTime);//keep a copy of pointer
	
	float max_angle_difference = 0;
	int boneNum = mainKey->boneRefs.size();
	int objectNum = mainKey->objectRefs.size();
	bool firstTime = timelineSlice.empty();

	for (int b = 0; b < boneNum; b++)
	{
		if (firstTime) {
			timelineSlice.push_back(new BoneTimelineKey());
		}
		//create the new timelineKey to fill up the timelineSlice
		TimelineKey*currentKey = timelineSlice[b];// new BoneTimelineKey();
		KeyFromRef(currentKey, mainKey->boneRefs[b], newTime);

		//for the fist time the prev slice will be empty
		if (sliceA == NULL)continue;

		//calculate the angle difference here.
		float angleA = ((BoneTimelineKey*)sliceA->at(b))->info.angle;
		float angleB = ((BoneTimelineKey*)currentKey)->info.angle;

		if (std::abs(angleB - angleA)>180.0f) {
			if (angleA > angleB) angleA -= 360.0f;
			else angleA += 360.0f;
		}
		float angle_difference = std::abs(angleB - angleA);
		if (max_angle_difference < angle_difference)
			max_angle_difference = angle_difference;
	}

	for (int o = 0; o < objectNum; o++) {

		if (firstTime) {
			timelineSlice.push_back(new SpriteTimelineKey());
		}
		//create the new timelineKey to fill up the timelineSlice
		TimelineKey*currentKey = timelineSlice[boneNum + o];// new SpriteTimelineKey();
		KeyFromRef(currentKey, mainKey->objectRefs[o], newTime);

		//for the fist time the prev slice will be empty
		if (sliceA == NULL)continue;

		//calculate the angle difference here.
		float angleA = ((SpriteTimelineKey*)sliceA->at(boneNum+o))->info.angle;
		float angleB = ((SpriteTimelineKey*)currentKey)->info.angle;
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

float Animation::UpdateAndDraw(float newTime)
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
	mainKey = MainlineKeyFromTime(newTime);//keep a copy of pointer
	UpdateCharacter(mainKey, newTime);
	return newTime;
}

#include"../../misc/Locator.h"
void Animation::UpdateCharacter(MainlineKey*mainKey, int newTime)
{
	int boneNum = mainKey->boneRefs.size();
	std::vector<BoneTimelineKey> transformBoneKeys(boneNum);
	for (int b = 0; b < boneNum; b++)
	{
		Ref currentRef = mainKey->boneRefs[b];
		KeyFromRef(timelineSlice[b], currentRef, newTime);
		m_pOwner->callback->UpdateBoneFromKey(b, (BoneTimelineKey*)timelineSlice[b]);
	}

	int objectNum = mainKey->objectRefs.size();
	for (int o = 0; o < objectNum; o++)
	{
		Ref currentRef = mainKey->objectRefs[o];
		KeyFromRef(timelineSlice[boneNum + o], currentRef, newTime);
		m_pOwner->callback->UpdateSpriteFromKey(o, (SpriteTimelineKey*)timelineSlice[boneNum + o]);
	}
	// <expose objectKeys to api users to retrieve AND replace objectKeys>

}

MainlineKey * Animation::MainlineKeyFromTime(int currentTime)
{
	int currentMainKey = 0;
	for (int m = 0, n=mainlineKeys.size(); m < n; m++)
	{
		if (mainlineKeys[m]->time <= currentTime)
		{
			currentMainKey = m;
		}
		if (mainlineKeys[m]->time >= currentTime)
		{
			break;
		}
	}
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
