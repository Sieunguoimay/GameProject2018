#include "Geometry.h"

void Geometry::Intersection(const b2Vec2 & c_p, const float & c_r, const b2Vec2 & _p1, const b2Vec2 & _p2, CirlceIntersectOutput * output)
{
	b2Vec2 p1 = _p1 - c_p;
	b2Vec2 p2 = _p2 - c_p;
	b2Vec2 d = p2-p1;
	float dr = d.Length();
	float D = p1.x*p2.y - p2.x*p1.y;

	float delta = c_r*c_r*dr*dr - D*D;
	output->type = -1;
	if (delta < 0) return;
	
	output->type = 0;
	if (delta > 0) {
		output->p1.x = c_p.x+(D*d.y - (d.y < 0 ? -1 : 1)*d.x*b2Sqrt(delta)) / (dr*dr);
		output->p1.y = c_p.y + (-D*d.x - b2Abs(d.y)*b2Sqrt(delta)) / (dr*dr);
		output->type = 1;
	}
	output->p2.x = c_p.x + (D*d.y + (d.y < 0 ? -1 : 1)*d.x*b2Sqrt(delta)) / (dr*dr);
	output->p2.y = c_p.y + (-D*d.x + b2Abs(d.y)*b2Sqrt(delta)) / (dr*dr);
}

bool Geometry::onLine(const b2Vec2 & a, const b2Vec2 & b1, const b2Vec2 & b2)
{
	if (a.x > std::max(b1.x, b2.x))return false;
	if (a.x < std::min(b1.x, b2.x))return false;
	if (a.y > std::max(b1.y, b2.y))return false;
	if (a.y < std::min(b1.y, b2.y))return false;
	return true;
}

int Geometry::direction(const b2Vec2 & a, const b2Vec2 & b, const b2Vec2 & c)
{
	float val = (b.y - a.y)*(c.x - b.x) - (b.x - a.x)*(c.y - b.y);
	if (val == 0)return 0;//colinear
	else if (val < 0)return -1;//anti-clockwise
	return 1;//clockwise
}

bool Geometry::CheckSegmentIntersect(const b2Vec2 & a1, const b2Vec2 & a2, const b2Vec2 & b1, const b2Vec2 & b2)
{
	int dir1 = direction(a1, a2, b1);
	int dir2 = direction(a1, a2, b2);
	int dir3 = direction(b1, b2, a1);
	int dir4 = direction(b1, b2, a2);

	if (dir1 != dir2 && dir3 != dir4) return true;
	if (dir1 == 0 && onLine(b1, a1, a2)) return true;
	if (dir2 == 0 && onLine(b2, a1, a2)) return true;
	if (dir3 == 0 && onLine(a1, b1, b2)) return true;
	if (dir4 == 0 && onLine(a2, b1, b2)) return true;

	return false;
}


float _linear(float a, float b, float t)
{
	return ((b - a)*t) + a;
}

float quadratic(float a, float b, float c, float t)
{
	return _linear(_linear(a, b, t), _linear(b, c, t), t);
}
float cubic(float a, float b, float c, float d, float t)
{
	return _linear(quadratic(a, b, c, t), quadratic(b, c, d, t), t);
}


float quartic(float a, float b, float c, float d, float e, float t)
{
	return _linear(cubic(a, b, c, d, t), cubic(b, c, d, e, t), t);
}

float quintic(float a, float b, float c, float d, float e, float f, float t)
{
	return _linear(quartic(a, b, c, d, e, t), quartic(b, c, d, e, f, t), t);
}


void normalizeDegreeAngle(float& angle) {
	if (angle < 0) angle += 360;
	else if (angle >= 360) angle -= 360;
}