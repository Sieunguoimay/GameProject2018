#pragma once
#include"Box2D\Box2D.h"
#include<functional>
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
			BezierCurve2(p1, p2, variance),
			BezierCurve2(p2, p3, variance), variance);
	}
	inline static b2Vec2 BezierCurve4(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3, b2Vec2 p4, float variance) {
		return BezierCurve2(
			BezierCurve3(p1, p2,p3, variance), 
			BezierCurve3(p2, p3,p4, variance), variance);
	}
	inline static b2Vec2 GetMidPointByX(float x, const b2Vec2&a, const b2Vec2&b) {
		if (x == a.x)return a;
		if (x == b.x)return b;
		return b2Vec2(x, (x-a.x)*(b.y-a.y)/(b.x-a.x)+a.y);
	}
	inline static b2Vec2 GetMidPointByY(float y, const b2Vec2&a, const b2Vec2&b) {
		if (y == a.y)return a;
		if (y == b.y)return b;
		return b2Vec2((y - a.y)*(b.x - a.x) / (b.y - a.y) + a.x,y);
	}
	static bool onLine(const b2Vec2&a, const b2Vec2&b1, const b2Vec2&b2);
	static int direction(const b2Vec2&a, const b2Vec2&b, const b2Vec2&c);
	static bool CheckSegmentIntersect(const b2Vec2&a1, const b2Vec2&a2, const b2Vec2&b1, const b2Vec2&b2);
	static bool LineSegmentIntersection(const b2Vec2&l11, const b2Vec2&l12, const b2Vec2&l21, const b2Vec2&l22, b2Vec2&intersect);
	static void normalizeDegreeAngle(float&angle);

	static void clipPolygonAgainstAABB(b2PolygonShape*shape, const b2AABB& aabb
		, std::function<void(const b2Vec2&)>addVertexToContainer
		, std::function<b2Vec2(const b2Vec2&)>localToWorld = NULL
	);

};

extern float _linear(float a, float b, float t);
extern float angle_linear(float angleA, float angleB, int spin, float t);
extern float quadratic(float a, float b, float c, float t);
extern float cubic(float a, float b, float c, float d, float t);
extern float quartic(float a, float b, float c, float d, float e, float t);
extern float quintic(float a, float b, float c, float d, float e, float f, float t);

