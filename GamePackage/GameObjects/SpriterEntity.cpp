#include "SpriterEntity.h"
#include<algorithm>
#include<limits>
#include"SpriterEngine\AnimationSwitcher.h"
#include"../misc/Math/Geometry.h"
#include"../misc/Locator.h"

//used for Spawner
SpriterEntity::SpriterEntity(ScmlEntity*entity, std::vector<SpriterEngine::Folder*>*pFolders)
	:m_pScmlEntity(entity)
	,m_pFolders(pFolders)
{}

//used for copy
SpriterEntity::SpriterEntity(SpriterEntity*spriterEntity, bool calculateAABB /*= false*/)
	: m_pScmlEntity(spriterEntity->m_pScmlEntity)
	, m_pFolders(spriterEntity->m_pFolders)
{

	auto&boneRefs = ((Animation*)m_pScmlEntity->animations[0])->mainlineKeys[0]->boneRefs;
	auto&spriteRefs = ((Animation*)m_pScmlEntity->animations[0])->mainlineKeys[0]->objectRefs;
	m_boneSegments.reserve(boneRefs.size());
	m_spriteSegments.reserve(spriteRefs.size());
	for (auto&a : boneRefs) m_boneSegments.push_back(new BoneSegment(m_boneSegments.size(), a.parent));
	for (auto&a : spriteRefs) m_spriteSegments.push_back(new SpriteSegment(m_spriteSegments.size(), a.parent, a.z_index));

	//each spriterEntity has only one animationSwitcher used for multiple times. 
	m_animationSwitcherIndex = m_pScmlEntity->animations.size();
	m_pScmlEntity->animations.push_back(new AnimationSwitcher(&m_boneSegments, m_animationSwitcherIndex,this));

	if (calculateAABB) {
		m_defaultAABB = glm::vec4(
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::min(),
			std::numeric_limits<float>::min());

		SetAnimation(0);
		Update(0);
		Draw();

		m_defaultOriginInAABB = glm::vec2(characterInfo.x, characterInfo.y) - glm::vec2(m_defaultAABB.x, m_defaultAABB.y);
	}
}
SpriterEntity::~SpriterEntity() {
	//erasing what you messed up at the beginning

	//get the temp animation for swapping
	if (m_animationIndex != -1)
	{
		//correct the index going tobe kept in the last element while jumping to the new location in the vector
		((AnimationSwitcher*)(*(m_pScmlEntity->animations.end() - 1)))->SetAnimationSwitcherIndex(m_animationSwitcherIndex);

		//swap
		std::iter_swap(m_pScmlEntity->animations.begin() + m_animationSwitcherIndex, m_pScmlEntity->animations.end() - 1);

		//destroy the last one
		delete *(m_pScmlEntity->animations.end() - 1);//delete the dynamic memory
		
		m_pScmlEntity->animations.erase(m_pScmlEntity->animations.end() - 1);//remove the last element 
	}
	for (auto&a : m_boneSegments) delete a;
	for (auto&a : m_spriteSegments) delete a;
	for (auto&a : m_timelineSlice) delete a;
}




bool SpriterEntity::HasDone() {
	if (m_animationIndex == -1) return false;
	if (m_animationIndex != m_animationSwitcherIndex)
		return m_pScmlEntity->animations.at(m_animationIndex)->HasDone();
	return false;
}

void SpriterEntity::Update(float deltaTime)
{
	m_currentTime += (int)(deltaTime*1000.0f);
	if (m_animationIndex == -1) return;
	if (m_animationIndex == m_animationSwitcherIndex) {
		AnimationSwitcher*temp = (AnimationSwitcher*)(m_pScmlEntity->animations.at(m_animationIndex));
		if (temp->HasDone())//gets the out going signal
		{
			m_animationIndex = temp->GetNextAnimationIndex();
			m_actualAnimationIndex = m_animationIndex;
			m_currentMainlineKeyIndex = 0;
		}
	}
	m_currentTime = m_pScmlEntity->animations.at(m_animationIndex)->Update(m_currentTime, &m_currentMainlineKeyIndex, this);
}

void SpriterEntity::Draw()
{
	if (m_animationIndex == -1) return;
	
	m_pScmlEntity->animations.at(m_animationIndex)->Draw(m_currentTime, this);

	if (m_aabbFlag) {
		m_aabbFlag = false;
		m_AABB = m_defaultAABB;
	}
	for (auto&a : m_boneSegments) {
		glm::vec2 dir(cos(glm::radians(a->GetInfo().angle)), sin(glm::radians(a->GetInfo().angle)));
		glm::vec2 pos(a->GetInfo().x, a->GetInfo().y);
		Locator::GetPrimitiveRenderer()->DrawLine(pos, pos + 20.0f*dir, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

}







BoneSegment* SpriterEntity::GetBodySegment(int boneIndex)
{
	if(boneIndex<m_boneSegments.size())
		return m_boneSegments[boneIndex];
	return NULL;
}


void SpriterEntity::UpdateBoneFromKey(int boneIndex, BoneTimelineKey*boneKey) {
	BoneSegment*segment = m_boneSegments[boneIndex];


	float oldAngle = segment->GetAngle();
	segment->SetInfo(boneKey->m_info);
	SpatialInfo&segmentInfo = segment->GetInfo();
	SpatialInfo parentInfo;
	if (segment->GetParentId() < 0) {
		parentInfo = characterInfo;
		if (m_flip == HORIZONTAL_FLIP) {
			segmentInfo.angle = 180.0f - segmentInfo.angle;
			Geometry::normalizeDegreeAngle(segmentInfo.angle);
			segmentInfo.x = -segmentInfo.x;
		}else if (m_flip == VERTICAL_FLIP) {
			//segment->GetInfo().angle = -segment->GetInfo().angle;
			//normalizeDegreeAngle(segment->GetInfo().angle);
			//info.y = -info.y;
		}
	}
	else {
		parentInfo = m_boneSegments[segment->GetParentId()]->GetInfo();
		segment->Flip(m_flip);
	}
	segmentInfo = segmentInfo.unmapFromParent(parentInfo);
	Geometry::normalizeDegreeAngle(segmentInfo.angle);

	if (segment->IsControlled()) {
		segment->SetAngle(oldAngle);
		float localAngle = oldAngle - m_boneSegments[segment->GetParentId()]->GetInfo().angle;
		((BoneTimelineKey*)m_timelineSlice[boneIndex])->m_info.angle = 
			(m_flip == HORIZONTAL_FLIP ?-localAngle: localAngle);
	}
	segment->Update();
}

void SpriterEntity::UpdateSpriteFromKey(int spriteIndex, SpriteTimelineKey*spriteKey) {
	Texture*&pTexture = m_pFolders->at(spriteKey->m_folderId)->files[spriteKey->m_fileId].pTexture;

	SpriteSegment*sprite = m_spriteSegments[spriteIndex];
	sprite->SetInfo(spriteKey->m_info, spriteKey->m_pivot_x, spriteKey->m_pivot_y, spriteKey->m_useDefaultPivot,pTexture);
	sprite->Flip(m_flip);

	SpatialInfo&info = sprite->GetInfo();
	info = info.unmapFromParent(m_boneSegments[sprite->GetParentId()]->GetInfo());

	sprite->Draw(m_flip, m_aabbFlag);

	if (m_aabbFlag) {
		const glm::vec4& aabb = sprite->GetAABB();
		if (m_defaultAABB.x > aabb.x)m_defaultAABB.x = aabb.x;
		if (m_defaultAABB.y > aabb.y)m_defaultAABB.y = aabb.y;
		if (m_defaultAABB.z < aabb.z)m_defaultAABB.z = aabb.z;
		if (m_defaultAABB.w < aabb.w)m_defaultAABB.w = aabb.w;
	}
}

TimelineKey*SpriterEntity::GetTimelineKeyFromSlice(int index) {
	if (index < m_boneSegments.size()) {
		if (m_boneSegments[index]->IsControlled()) {
			return &m_tempKey;
		}
	}
	return m_timelineSlice[index];
}


void SpriterEntity::SetAnimationSwitchingTime(const char*animationA, const char*animationB, int time)
{
	m_pScmlEntity->animationSwitchingTime
		.at(GetAnimationIndex(animationA))
		.at(GetAnimationIndex(animationB)) = time;
}

void SpriterEntity::SetPos(const glm::vec2 & pos)
{
	if (m_updatePositionFlag) {
		characterInfo.x = pos.x;
		characterInfo.y = pos.y;

		glm::vec2 s(m_AABB.z - m_AABB.x, m_AABB.w - m_AABB.y);
		glm::vec2 p(
			characterInfo.x - m_defaultOriginInAABB.x* characterInfo.scaleX,
			characterInfo.y - m_defaultOriginInAABB.y* characterInfo.scaleY);
		m_AABB.x = p.x;
		m_AABB.y = p.y;
		m_AABB.z = p.x + s.x;
		m_AABB.w = p.y + s.y;
	}
}

void SpriterEntity::SetAABB(const glm::vec4 & AABB)
{
	characterInfo.scaleX = (AABB.z - AABB.x) / (m_defaultAABB.z - m_defaultAABB.x);
	characterInfo.scaleY = (AABB.w - AABB.y) / (m_defaultAABB.w - m_defaultAABB.y);
	characterInfo.x = (AABB.x) + m_defaultOriginInAABB.x*characterInfo.scaleX;
	characterInfo.y = (AABB.y) + m_defaultOriginInAABB.y*characterInfo.scaleY;
	m_AABB = AABB;
}

int SpriterEntity::GetSegmentIndex(const char * boneName)
{
	auto&map = m_pScmlEntity->bodySegmentMap;
	auto&pos = map.find(boneName);
	if (pos != map.end())
		return pos->second;
	return -1;
}

SpriterEntity * SpriterEntity::Spawn()
{
	return new SpriterEntity(this,true);
}


void SpriterEntity::SetAnimation(const char* name)
{
	m_currentTime = 0;
	auto animation = m_pScmlEntity->animationMap.find(name);
	if (animation == m_pScmlEntity->animationMap.end()) {
		SDL_Log("Animation %s name Not found. Set to default (0)", name);
		m_animationIndex = 0;
		m_actualAnimationIndex = 0;
		m_currentMainlineKeyIndex = 0;
		return;
	}
	SetAnimation(animation->second);
}

void SpriterEntity::SetAnimation(int newAnimationIndex, glm::vec2 jumpDistance /*= glm::vec2(0, 0)*/)
{
	m_currentTime = 0;
	// in case of the first time
	if (m_animationIndex == -1) {
		m_animationIndex = newAnimationIndex;
		m_actualAnimationIndex = newAnimationIndex;
		m_currentMainlineKeyIndex = 0;
		//just fill the of the first animation and do not care about the return value
		((Animation*)m_pScmlEntity->animations.at(m_animationIndex))->CalculateSliceAtZero(m_timelineSlice,NULL);
		return;
	}

	//take out the switching times from the container at the positions
	//of valid index of animations which are less than the size of 
	//animation container
	int switchingTime = -1;
	SDL_Log("m_animationIndex: %d %d", m_actualAnimationIndex, newAnimationIndex);
	if (m_actualAnimationIndex < m_pScmlEntity->animationMap.size()) {
		if (newAnimationIndex < m_pScmlEntity->animationMap.size()) {
			switchingTime = (m_pScmlEntity->animationSwitchingTime).at(m_actualAnimationIndex).at(newAnimationIndex);
		}
	}
	if (switchingTime == 0) {
		m_animationIndex = newAnimationIndex;
		m_actualAnimationIndex = newAnimationIndex;
		m_currentMainlineKeyIndex = 0;
		//just fill the of the first animation and do not care about the return value
		((Animation*)m_pScmlEntity->animations.at(m_animationIndex))->CalculateSliceAtZero(m_timelineSlice,NULL);
		return;
	}

	SDL_Log("Switching Time: %d ", switchingTime);
	//take out the animationSwitcher of this spriterEntity and init() it again
	//with the new pair of animations
	((AnimationSwitcher*)(m_pScmlEntity->animations.at(m_animationSwitcherIndex)))->Init(
		m_pScmlEntity->animations.at(m_animationIndex),//this animation maybe a switcher or an Animation Object.
		m_pScmlEntity->animations.at(newAnimationIndex),//the second animation here is properly a real Animation Object
		switchingTime, newAnimationIndex,jumpDistance);

	m_animationIndex = m_animationSwitcherIndex;
	m_actualAnimationIndex = newAnimationIndex;
}

int SpriterEntity::GetAnimationIndex(const char* name)
{
	auto animation = m_pScmlEntity->animationMap.find(name);
	if (animation == m_pScmlEntity->animationMap.end()) {
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