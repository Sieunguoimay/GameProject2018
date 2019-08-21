#include "TimelineKey.h"
#include<glm\glm.hpp>
#include"../../misc/Assistances.h"
#include"../../misc/Math/Geometry.h"

float angle_linear(float angleA, float angleB, int spin, float t)
{
	if (spin == 0)
	{
		return angleA;
	}
	if (spin>0)
	{
		if ((angleB - angleA)<0)
		{
			angleB += 360;
		}
	}
	else if (spin<0)
	{
		if ((angleB - angleA)>0)
		{
			angleB -= 360;
		}
	}

	return _linear(angleA, angleB, t);
}
SpatialInfo _linear(SpatialInfo infoA, SpatialInfo infoB, int spin, float t)
{
	SpatialInfo resultInfo;
	resultInfo.x = _linear(infoA.x, infoB.x, t);
	resultInfo.y = _linear(infoA.y, infoB.y, t);
	resultInfo.angle = angle_linear(infoA.angle, infoB.angle, spin, t);
	resultInfo.scaleX = _linear(infoA.scaleX, infoB.scaleX, t);
	resultInfo.scaleY = _linear(infoA.scaleY, infoB.scaleY, t);
	resultInfo.a = _linear(infoA.a, infoB.a, t);
	return resultInfo;
}




#include<iostream>
void TimelineKey::Log() {
	SDL_Log("\t\t\tkey %d %d %d %f %f\n", time, spin, curveType, c1, c2);
}
void BoneTimelineKey::Log() {
	TimelineKey::Log();
	SDL_Log("\t\t\tbone %f %f %f %f %f %f --- %d %d\n", info.x, info.y, info.angle, info.scaleX, info.scaleY,info.a, length, height);
}
void SpriteTimelineKey::Log() {
	TimelineKey::Log();
	SDL_Log("\t\t\tobject %f %f %f %f %f %f --- %d %d %f %f\n", info.x, info.y, info.angle, info.scaleX, info.scaleY, info.a, /*folder, file,*/ pivot_x, pivot_y);
}



void TimelineKey::Interpolate(TimelineKey*movingKey, TimelineKey*nextKey, int nextKeyTime, float currentTime)
{
	Linear(movingKey,nextKey, GetTWithNextKey(nextKey, nextKeyTime, currentTime));
}


float TimelineKey::GetTWithNextKey(TimelineKey*nextKey, int nextKeyTime, float currentTime)
{
	if (curveType == INSTANT || time == nextKeyTime)
	{
		return 0;
	}

	float t = (currentTime - (float)time) / (float)(nextKeyTime - time);
	if (curveType == LINEAR)
	{

		return t;
	}
	else if (curveType == QUADRATIC)
	{
		return(quadratic(0.0,c1,1.0,t));
	}
	else if (curveType == CUBIC)
	{
		return(cubic(0.0,c1,c2,1.0,t));
	}
	else if (curveType == QUARTIC)
	{
		return(quartic(0.0, c1, c2, c3,1.0, t));
	}
	else if (curveType == QUINTIC)
	{
		return(quintic(0.0, c1, c2,c3,c4, 1.0, t));
	}
	return 0; // Runtime should never reach here        
}


void TimelineKey::Clone(TimelineKey*target) {
	target->c1 = c1;
	target->c2 = c2;
	target->c3 = c3;
	target->c4 = c4;
	target->time = time;
	target->spin = spin;
	target->curveType = curveType;
}






SpatialInfo SpatialInfo::unmapFromParent(SpatialInfo parentInfo)
{

	SpatialInfo unmappedObj = *this;
	unmappedObj.angle += parentInfo.angle;
	unmappedObj.scaleX *= parentInfo.scaleX;
	unmappedObj.scaleY *= parentInfo.scaleY;
	unmappedObj.a *= parentInfo.a;
	if (x != 0 || y != 0)
	{
		float preMultX = x*parentInfo.scaleX;
		float preMultY = y*parentInfo.scaleY;
		float s = sin(glm::radians(parentInfo.angle));
		float c = cos(glm::radians(parentInfo.angle));

		unmappedObj.x = (preMultX * c) - (preMultY * s);
		unmappedObj.y = (preMultX * s) + (preMultY * c);

		unmappedObj.x += parentInfo.x;
		unmappedObj.y += parentInfo.y;
	}
	else
	{
		// Mandatory optimization for future features           
		unmappedObj.x = parentInfo.x;
		unmappedObj.y = parentInfo.y;
	}
	return unmappedObj;
}


//#include"ScmlObject.h"
//#include"../../misc/Locator.h"
//void SpriteTimelineKey::Paint( FlipType flip /*= FLIP_NONE*/)
//{
//	int paintPivotX;
//	int paintPivotY;
//	//SpriterEngine::File&textureFile = folders.at(folder)->files[file];//done ever copy such object having string member 
//	Texture*pTexture = m_pFile->pTexture;
//	if (pTexture == NULL) return;
//
//	if (useDefaultPivot)
//	{
//		paintPivotX = m_pFile->pivotX;
//		paintPivotY = m_pFile->pivotY;
//	}
//	else
//	{
//		paintPivotX = pivot_x;
//		paintPivotY = pivot_y;
//	}
//
//	// paint image represented by
//	// ScmlObject.activeCharacterMap[folder].files[file],fileReference 
//	// at x,y,angle (counter-clockwise), offset by paintPivotX,paintPivotY
//
//	float w = info.scaleX*((float)pTexture->GetWidth());
//	float h = info.scaleY*((float)pTexture->GetHeight());
//
//	//since pivot in spriter is at the top-left
//	//thankfully we have faced this problem before, so we just give the pivot
//	//position to the Draw() function as the center of rotating
//
//	glm::vec2 pivot(w*(float)paintPivotX, h*(float)paintPivotY);
//	
//	float x = info.x;
//	float y = info.y - h;
//	float angle = info.angle;
//
//	if (flip == HORIZONTAL_FLIP) {
//		pivot.x = w - pivot.x;
//		x = -w - info.x;
//		angle = 360.0f - info.angle;
//	}else if (flip == VERTICAL_FLIP) {
//		pivot.y = h - pivot.y;
//		y = -h - y;
//		angle = 360.0f - info.angle;
//	}
//
//	angle = glm::radians(angle);
//
//
//
//
//	const SpriteMesh*a=Locator::GetTextureRenderer()->Draw(
//		glm::vec4(x,y, w, h),
//		glm::vec4(0.0, 0.0, 1.0, 1.0), 
//		pTexture->GetId(),0, 
//		glm::vec4(1.0f), angle,
//		pivot,flip==HORIZONTAL_FLIP, flip == VERTICAL_FLIP);
//
//	if (a == NULL) {
//		m_AABB = glm::vec4(x, y, x, y);
//		return;
//	}
//	//calculate AABB
//	glm::vec2 p[3] = {a->bottomLeft.position,
//					a->topRight.position,a->bottomRight.position};
//
//
//	m_AABB = glm::vec4(a->topLeft.position, a->topLeft.position);
//	for (int i = 0; i < 3; i++) {
//		if (m_AABB.x > p[i].x) m_AABB.x = p[i].x;
//		else if (m_AABB.z < p[i].x) m_AABB.z = p[i].x;
//
//		if (m_AABB.y > p[i].y) m_AABB.y = p[i].y;
//		else if (m_AABB.w < p[i].y) m_AABB.w = p[i].y;
//	}
//
//	//if(s_drawAABBTrigger)
//	//Locator::GetPrimitiveRenderer()->DrawBox(
//	//	glm::vec4(m_AABB.x, m_AABB.y, m_AABB.z - m_AABB.x, m_AABB.w - m_AABB.y));
//
//}
//
//void BoneTimelineKey::Paint(FlipType flip /*= FLIP_NONE*/)
//{
//	if (paintDebugBones)
//	{
//		float drawLength = (float)length*info.scaleX;
//		float drawHeight = (float)height*info.scaleY;
//		// paint debug bone representation 
//		// e.g. line starting at x,y,at angle, 
//		// of length drawLength, and height drawHeight
//
//		float s = glm::sin(glm::radians(info.angle));
//		float c = glm::cos(glm::radians(info.angle));
//
//		glm::vec2 p1(info.x, info.y);
//		glm::vec2 p2 = p1 + glm::vec2(c*drawLength, s*drawLength);
//		if (flip == HORIZONTAL_FLIP) {
//			p1.x = -p1.x;
//			p2.x = -p2.x;
//		}
//		else if (flip == VERTICAL_FLIP) {
//			p1.y = -p1.y;
//			p2.y = -p2.y;
//		}
//		Locator::GetPrimitiveRenderer()->DrawLine(p1,p2);
//
//	}
//}
//

void BoneTimelineKey::Linear(TimelineKey*_movingKey,TimelineKey*_keyB, float t)
// keyB must be BoneTimelineKeys
{
	BoneTimelineKey*keyB = (BoneTimelineKey*)_keyB;
	BoneTimelineKey*movingKey = (BoneTimelineKey*)_movingKey;
	this->Clone(movingKey);

	movingKey->info = _linear(info, keyB->info, spin, t);
	if (paintDebugBones)
	{
		movingKey->length = _linear((float)length, (float)keyB->length, t);
		movingKey->height = _linear((float)height, (float)keyB->height, t);
	}

}

void BoneTimelineKey::Clone(TimelineKey * target)
{
	*((BoneTimelineKey*)target) = *this;
}
TimelineKey * BoneTimelineKey::Spawn()
{
	return new BoneTimelineKey();
}
void BoneTimelineKey::Unmap(SpatialInfo parentInfo)
{
	this->info = this->info.unmapFromParent(parentInfo);
}
void SpriteTimelineKey::Linear(TimelineKey*movingKey,TimelineKey*keyB, float t)
{
	SpriteTimelineKey*_keyB = ((SpriteTimelineKey*)keyB);
	SpriteTimelineKey*_movingKey = ((SpriteTimelineKey*)movingKey);
	this->Clone(_movingKey);

	_movingKey->info = _linear(info, _keyB->info, spin, t);
	if (!useDefaultPivot)
	{
		_movingKey->pivot_x = _linear(pivot_x, _keyB->pivot_x, t);
		_movingKey->pivot_y = _linear(pivot_y, _keyB->pivot_y, t);
	}
}

void SpriteTimelineKey::Clone(TimelineKey * target)
{
	*((SpriteTimelineKey*)target) = *this;
}


TimelineKey * SpriteTimelineKey::Spawn()
{
	return new SpriteTimelineKey();
}
void SpriteTimelineKey::Unmap(SpatialInfo parentInfo)
{
	this->info = this->info.unmapFromParent(parentInfo);
}
