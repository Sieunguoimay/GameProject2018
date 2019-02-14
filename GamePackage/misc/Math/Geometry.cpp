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
