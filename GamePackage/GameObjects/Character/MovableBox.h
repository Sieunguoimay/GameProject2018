#pragma once
#include"TouchPoint.h"
#include"../EntityHierachy/HavingBodyEntity.h"
class MovableBox :public NoAnimationBodyEntity{
	TouchPoint m_touchPoints[4];
public:
	MovableBox(NoAnimationSkin*skin/*, ObjectType*/, glm::vec4 AABB);
	void SetupBody(bool atRunTime=false)override;
	//inline const TouchPoint&GetTouchPoint() const { return m_touchPoint; }
};