#include "SpriterEntity.h"
#include<algorithm>
#include<limits>
#include"SpriterEngine\AnimationSwitcher.h"


//used for Spawner
SpriterEntity::SpriterEntity(ScmlObject*pScmlObject, int scmlEntityIndex)
{
	//once for all
	m_pScmlObject = pScmlObject;
	m_scmlEntityIndex = scmlEntityIndex;
	m_animationMap = &(m_pScmlObject->entities[m_scmlEntityIndex]->animationMap);
	m_animationContainer = &(m_pScmlObject->entities[m_scmlEntityIndex]->animations);
	m_actualAnimationNum = m_animationContainer->size();
	m_animationIndex = -1;
	m_actualAnimationIndex = -1;
}
//used for copy
SpriterEntity::SpriterEntity(SpriterEntity*spriterEntity)
{
	m_pScmlObject = spriterEntity->m_pScmlObject;
	m_scmlEntityIndex = spriterEntity->m_scmlEntityIndex;
	m_animationMap = spriterEntity->m_animationMap;
	m_animationContainer = spriterEntity->m_animationContainer;
	m_actualAnimationNum = spriterEntity->m_actualAnimationNum;
	m_animationIndex = -1;
	m_actualAnimationIndex = -1;

	auto&boneRefs = ((Animation*)m_pScmlObject->entities[m_scmlEntityIndex]->animations[0])->mainlineKeys[0]->boneRefs;
	for (auto&a : boneRefs)
		bodySegments.push_back(new BodySegment(bodySegments.size(), a.parent));
	auto&spriteRefs = ((Animation*)m_pScmlObject->entities[m_scmlEntityIndex]->animations[0])->mainlineKeys[0]->objectRefs;
	for (auto&a : spriteRefs)
		spriteSegments.push_back(new SpriteSegment(spriteSegments.size(), a.parent,a.z_index));


	//each spriterEntity has only one animationSwitcher used for multiple times. 
	m_animationContainer->push_back(new AnimationSwitcher(m_pScmlObject,m_pScmlObject->entities[m_scmlEntityIndex],(void*)this, &bodySegments));
	m_animationSwitcherIndex = m_animationContainer->size() - 1;
	
	//query for callback at animation update
	m_pScmlObject->SetAnimationCallback(this);
}
SpriterEntity::~SpriterEntity() {
	//erasing what you messed up at the beginning

	//get the temp animation for swapping
	if (m_animationIndex != -1)
	{
		//correct the index going tobe kept in the last element while jumping to the new location in the vector
		((SpriterEntity*)(((AnimationSwitcher*)(*(m_animationContainer->end() - 1)))->GetOwner()))->m_animationSwitcherIndex = m_animationSwitcherIndex;

		//swap
		std::iter_swap(m_animationContainer->begin() + m_animationSwitcherIndex, m_animationContainer->end() - 1);

		//destroy the last one
		delete *(m_animationContainer->end() - 1);//delete the dynamic memory
		m_animationContainer->erase(m_animationContainer->end() - 1);//remove the last element 
	}
	for (auto&a : bodySegments) delete a;
	for (auto&a : spriteSegments) delete a;

}



#include"../misc/Locator.h"

bool SpriterEntity::HasDone() {
	if (m_animationIndex == -1)
		return false;

	if (m_animationIndex != m_animationSwitcherIndex)
		return m_animationContainer->at(m_animationIndex)->HasDone();
	
	return false;
}

void SpriterEntity::SetInfo(glm::vec2 pos, float angle, float scaleX, float scaleY, FlipType flip) {
	characterInfo.set(pos.x, pos.y, angle, scaleX, scaleY, 1);
	m_flip = flip;
}
void SpriterEntity::Update(float deltaTime)
{
	m_currentTime += (int)(deltaTime*1000.0f);
	if (m_animationIndex == -1) return;
	if (m_animationIndex == m_animationSwitcherIndex) {
		AnimationSwitcher*temp = (AnimationSwitcher*)(m_animationContainer->at(m_animationIndex));
		if (temp->HasDone())//gets the out going signal
		{
			m_animationIndex = temp->GetNextAnimationIndex();
			m_actualAnimationIndex = m_animationIndex;
		}
	}

	m_pScmlObject->currentEntity = m_scmlEntityIndex;
	m_pScmlObject->currentAnimation = m_animationIndex;

	m_AABB = glm::vec4(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	m_currentTime = m_pScmlObject->Draw(m_currentTime);

}

void SpriterEntity::Draw()
{
	if (m_animationIndex == -1) return;

	//for (auto&a : bodySegments) {
	//	glm::vec2 dir(cos(glm::radians(a->GetInfo().angle)), sin(glm::radians(a->GetInfo().angle)));
	//	glm::vec2 pos(a->GetInfo().x, a->GetInfo().y);
	//	Locator::GetPrimitiveRenderer()->DrawLine(pos, pos + 20.0f*dir, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//}
	for (auto&a : spriteSegments) {
		a->Draw(m_flip);
		const glm::vec4& aabb = a->GetAABB();
		if (m_AABB.x > aabb.x)m_AABB.x = aabb.x;
		if (m_AABB.y > aabb.y)m_AABB.y = aabb.y;
		if (m_AABB.z < aabb.z)m_AABB.z = aabb.z;
		if (m_AABB.w < aabb.w)m_AABB.w = aabb.w;
		
	}
}







BodySegment* SpriterEntity::GetBodySegment(int boneIndex)
{
	if(boneIndex<bodySegments.size())
		return bodySegments[boneIndex];
	return NULL;
}


#include"../misc/Math/Geometry.h"
void SpriterEntity::UpdateBoneFromKey(int boneIndex, BoneTimelineKey*boneKey) {
	BodySegment*segment = bodySegments[boneIndex];
	float angle = segment->GetAngle();
	segment->SetInfo(boneKey->info);

	SpatialInfo&info = segment->GetInfo();
	SpatialInfo parentInfo;
	if (segment->GetParentId() < 0) {
		parentInfo = characterInfo;
		if (m_flip == HORIZONTAL_FLIP) {
			info.angle = 180.0f - info.angle;
			normalizeDegreeAngle(info.angle);
		}else if (m_flip == VERTICAL_FLIP) {
			//segment->GetInfo().angle = -segment->GetInfo().angle;
			//normalizeDegreeAngle(segment->GetInfo().angle);
		}
	}
	else {
		parentInfo = bodySegments[segment->GetParentId()]->GetInfo();
		segment->Update(m_flip);
	}
	info = info.unmapFromParent(parentInfo);
	normalizeDegreeAngle(info.angle);

	if (segment->IsControlled())
		segment->SetAngle(angle);
}
void SpriterEntity::UpdateSpriteFromKey(int spriteIndex, SpriteTimelineKey*spriteKey) {
	Texture*&pTexture = m_pScmlObject->folders[spriteKey->folderId]->files[spriteKey->fileId].pTexture;
	
	SpriteSegment*sprite = spriteSegments[spriteIndex];
	sprite->SetInfo(spriteKey->info, spriteKey->pivot_x, spriteKey->pivot_y, spriteKey->useDefaultPivot,pTexture);
	sprite->Update(m_flip);

	SpatialInfo&info = sprite->GetInfo();
	info = info.unmapFromParent(bodySegments[sprite->GetParentId()]->GetInfo());

}







void SpriterEntity::SetAnimationSwitchingTime(const char*animationA, const char*animationB, int time)
{
	m_pScmlObject->entities[m_scmlEntityIndex]->animationSwitchingTime
		.at(GetAnimationIndex(animationA))
		.at(GetAnimationIndex(animationB)) = time;
}

int SpriterEntity::GetSegmentIndex(const char * boneName)
{
	auto&map = m_pScmlObject->entities[m_scmlEntityIndex]->bodySegmentMap;
	auto&pos = map.find(boneName);
	if (pos != map.end())
		return pos->second;
	return -1;
}

SpriterEntity * SpriterEntity::Spawn()
{
	return new SpriterEntity(this);
}


void SpriterEntity::SetAnimation(const char* name)
{
	m_currentTime = 0;
	auto animation = m_animationMap->find(name);
	if (animation == m_animationMap->end()) {
		SDL_Log("Animation %s name Not found. Set to default (0)", name);
		m_animationIndex = 0;
		m_actualAnimationIndex = 0;
		return;
	}
	SetAnimation(animation->second);
}

void SpriterEntity::SetAnimation(int newAnimationIndex)
{
	m_currentTime = 0;
	// in case of the first time
	if (m_animationIndex == -1) {
		m_animationIndex = newAnimationIndex;
		m_actualAnimationIndex = newAnimationIndex;
		//just fill the of the first animation and do not care about the return value
		((Animation*)m_animationContainer->at(m_animationIndex))->CalculateSliceAtZero(NULL);
		return;
	}




	//take out the switching times from the container at the positions
	//of valid index of animations which are less than the size of 
	//animation container
	int switchingTime = -1;
	if (m_animationIndex<m_animationMap->size() && newAnimationIndex <m_animationMap->size())
		switchingTime = 
			(m_pScmlObject->entities[m_scmlEntityIndex]->animationSwitchingTime)
			.at(m_animationIndex).at(newAnimationIndex);
	//SDL_Log("Switching Time: %d ", switchingTime);
	//take out the animationSwitcher of this spriterEntity and init() it again
	//with the new pair of animations
	((AnimationSwitcher*)(m_animationContainer->at(m_animationSwitcherIndex)))->Init(
		m_animationContainer->at(m_animationIndex),//this animation maybe a switcher or an Animation Object.
		m_animationContainer->at(newAnimationIndex),//the second animation here is properly a real Animation Object
		switchingTime, newAnimationIndex);

	m_animationIndex = m_animationSwitcherIndex;
	m_actualAnimationIndex = newAnimationIndex;
}

int SpriterEntity::GetAnimationIndex(const char* name)
{
	auto animation = m_animationMap->find(name);
	if (animation == m_animationMap->end()) {
		SDL_Log("Animation %s name Not found. Set to default (0)", name);
		return -1;
	}
	return animation->second;
}






//
//Timeline * SpriterEntity::GetTimeline(const char * animationName, const char * timelineName)
//{
//	int animationIndex = GetAnimationIndex(animationName);
//	Animation*animation = ((Animation*)(GetSclmEntity()->animations[animationIndex]));
//	Timeline*timeline = animation->timelines[animation->timelineMap.find(timelineName)->second];
//	//TakeControl(timeline);
//	return timeline;
//}
//
//int SpriterEntity::GetTimeline(int animationIndex, const char * timelineName)
//{
//	Animation*animation = ((Animation*)(GetSclmEntity()->animations[animationIndex]));
//	return animation->timelineMap.find(timelineName)->second;
//	//Timeline*timeline = animation->timelines[animation->timelineMap.find(timelineName)->second];
//	//TakeControl(timeline);
//	//return timeline;
//}

//Timeline * SpriterEntity::GetTimeline(int animationIndex, int timelineIndex)
//{
//	Timeline*timeline = ((Animation*)(GetSclmEntity()->animations[animationIndex]))
//		->timelines[timelineIndex];
//	//TakeControl(timelineIndex);
//	return timeline;
//}
//
//
//void SpriterEntity::SetTimeToZero()
//{
//	m_currentTime = 0;
//}


//
////take control over the given timeline
//void SpriterEntity::TakeControl(int boneIndex)
//{
//	if (!m_pControlKeys->at(boneIndex)->IsControlled()) {
//		float angle = ((BoneTimelineKey*)(m_animationContainer->at(m_animationIndex)->GetSlice()[boneIndex]))->info.angle;
//		m_pControlKeys->at(boneIndex)->TakeControl(angle);
//	}
//}
//void SpriterEntity::ReleaseControl(int boneIndex)
//{
//	m_pControlKeys->at(boneIndex)->Release();
//}
//void SpriterEntity::ControlAngle(int boneIndex,float angle)
//{
//	m_pControlKeys->at(boneIndex)->SetAngle(angle);
//}


//ScmlEntity * SpriterEntity::GetSclmEntity()
//{
//	return m_pScmlObject->entities[m_scmlEntityIndex];
//}

//void SpriterEntity::Draw()
//{
//	Draw(glm::vec2(0, 0), 0.0f, 1.0f);
//}