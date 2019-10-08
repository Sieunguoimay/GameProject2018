#pragma once
#include<iostream>
#include"Folder.h"
using namespace SpriterEngine;
enum FlipType { FLIP_NONE, VERTICAL_FLIP, HORIZONTAL_FLIP };
class TimelineKey
{
public:
	enum  CurveType { INSTANT, LINEAR, QUADRATIC, CUBIC , QUARTIC, QUINTIC};
	int time = 0;
	int spin = 1;
	int curveType = LINEAR; // enum : INSTANT,LINEAR,QUADRATIC,CUBIC
	float c1;
	float c2;
	float c3;
	float c4;

	virtual void Log();

	//TimelineKey*Interpolate(TimelineKey*nextKey, int nextKeyTime, float currentTime);
	void Interpolate(TimelineKey*movingKey,TimelineKey*nextKey,int nextKeyTime, float currentTime);
	float GetTWithNextKey(int nextKeyTime, float currentTime);

	// overridden in inherited types  return linear(this,keyB,t);
	//virtual void Paint( FlipType flip = FLIP_NONE) {}
	virtual void Linear(TimelineKey*movingKey,TimelineKey*keyB, float t) {  }
	virtual void Clone(TimelineKey*target);
	virtual TimelineKey*Spawn()=0;
	virtual void Unmap(struct SpatialInfo)=0;
};

struct SpatialInfo
{
	float x = 0;
	float y = 0;
	float angle = 0;
	float scaleX = 1;
	float scaleY = 1;
	float a = 1; //alpha =)

	SpatialInfo unmapFromParent(SpatialInfo parentInfo);
	static SpatialInfo linear(SpatialInfo infoA, SpatialInfo infoB, int spin, float t);
};



class SpatialTimelineKey : public TimelineKey
{
public:
	SpatialInfo m_info;
};


class BoneTimelineKey: public SpatialTimelineKey
{
private:
	// unimplemented in Spriter
	int length = 50;
	int height = 5;
	bool paintDebugBones = false;

	void Log()override;

	// override paint if you want debug visuals for bones
	//void Paint( FlipType flip = FLIP_NONE)override;
	void Linear(TimelineKey*movingKey, TimelineKey*keyB, float t)override;
	void Clone(TimelineKey*target) override;
	TimelineKey*Spawn()override;
	void Unmap(SpatialInfo parentInfo)override;

	friend class ScmlObject;
	friend class Animation;
	friend class AnimationSwitcher;
	friend class SpriterEntity;
};

class SpriteTimelineKey: public SpatialTimelineKey
{
public:
	bool m_useDefaultPivot; // true if missing pivot_x and pivot_y in object tag
	float m_pivot_x = 0;
	float m_pivot_y = 1;


private:
	//what is the purpose of using these two???-> just to retrieve texture???
	//int folder; // index of the folder within the ScmlObject
	//int file;

	//question is: how the texture is changed? 
	//->it depends on when the file and folder are changed. at that point,
	//simply call setFile() instead.
	int m_folderId;
	int m_fileId;

	void Log()override;

	//void Paint( FlipType flip = FLIP_NONE)override;
	void Linear(TimelineKey*movingKey, TimelineKey*keyB, float t)override;
	void Clone(TimelineKey*target) override;
	void Unmap(SpatialInfo parentInfo)override;
	TimelineKey*Spawn()override;

	friend class ScmlObject;
	friend class Animation;
	friend class AnimationSwitcher;
	friend class SpriterEntity;
};

