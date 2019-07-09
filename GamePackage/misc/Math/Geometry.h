#pragma once
#include"Box2D\Box2D.h"
class Geometry {
public:
	struct CirlceIntersectOutput {
		int type;//-1 no, 1 yes 2, 0 yes 1
		b2Vec2 p1;
		b2Vec2 p2;
	};
	static void Intersection(const b2Vec2& c_p, const float& c_r, const b2Vec2& p1, const b2Vec2& p2,CirlceIntersectOutput*output);

	inline static b2Vec2 BezierCurve2(b2Vec2 p1, b2Vec2 p2, float variance) { 
		return b2Vec2(p1 + variance*(p2 - p1)); 
	}
	inline static b2Vec2 BezierCurve3(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3, float variance){
		return BezierCurve2(
			BezierCurve2(p1,p2, variance), 
			BezierCurve2(p2,p3,variance), variance);
	}
	inline static b2Vec2 BezierCurve4(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3, b2Vec2 p4, float variance) {
		return BezierCurve2(
			BezierCurve3(p1, p2,p3, variance), 
			BezierCurve3(p2, p3,p4, variance), variance);
	}
};

extern float _linear(float a, float b, float t);
extern float angle_linear(float angleA, float angleB, int spin, float t);
extern float quadratic(float a, float b, float c, float t);
extern float cubic(float a, float b, float c, float d, float t);
extern float quartic(float a, float b, float c, float d, float e, float t);
extern float quintic(float a, float b, float c, float d, float e, float f, float t);

extern void normalizeDegreeAngle(float&angle);