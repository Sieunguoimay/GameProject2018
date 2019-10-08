#include "Geometry.h"
#include"../Assistances.h"
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

bool Geometry::LineSegmentIntersection(const b2Vec2&l11, const b2Vec2&l12, const b2Vec2&l21, const b2Vec2&l22,b2Vec2&intersection)
{
	b2Vec2 d = l12 - l11;
	float m1 = d.y / d.x;
	float  c1 = l11.y - m1 * l11.x; // which is same as y2 - slope * x2
	
	d = l22 - l21;
	float m2 = d.y / d.x;
	float  c2 = l21.y - m2 * l21.x; // which is same as y2 - slope * x2

	if ((m1 - m2) == 0)
		return false;
	else
	{
		intersection.x = (c2 - c1) / (m1 - m2);
		intersection.y = m1 * intersection.x + c1;
	}
	return true;
}

void Geometry::normalizeDegreeAngle(float& angle) {
	if (angle < 0) angle += 360;
	else if (angle >= 360) angle -= 360;
}


void Geometry::clipPolygonAgainstAABB(
	b2PolygonShape*shape, const b2AABB& clippingAABB,
	std::function<void(const b2Vec2&)>addVertexToContainer,
	std::function<b2Vec2(const b2Vec2&)>localToWorld /*= NULL*/)
{
	//collect vertices
	int n = shape->m_count;
	b2Vec2 topLeft{ clippingAABB.lowerBound.x, clippingAABB.upperBound.y };
	b2Vec2 bottomRight{ clippingAABB.upperBound.x, clippingAABB.lowerBound.y };
	b2Vec2*vertices = shape->m_vertices;
	b2Vec2 lastVertex;
	bool lastInside = false;

	for (int i = 0; i < n + 1; i++) {
		b2Vec2 vertex = vertices[i == n ? 0 : i];
		if (localToWorld != NULL) vertex = localToWorld(vertex);

		if (Utils::check_AABB_against_point(clippingAABB, vertex)) {
			if (!lastInside&&i != 0) {
				float x = (lastVertex.x < vertex.x ? clippingAABB.lowerBound.x : clippingAABB.upperBound.x);
				float y = (lastVertex.y < vertex.y ? clippingAABB.lowerBound.y : clippingAABB.upperBound.y);
				b2Vec2 a;
				if (Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(x, clippingAABB.lowerBound.y), b2Vec2(x, clippingAABB.upperBound.y)))
					a = Geometry::GetMidPointByX(x, lastVertex, vertex);
				else if (Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(clippingAABB.lowerBound.x, y), b2Vec2(clippingAABB.upperBound.x, y)))
					a = Geometry::GetMidPointByY(y, lastVertex, vertex);
				addVertexToContainer(a);
			}
			lastVertex = vertex;
			lastInside = true;
		}
		else {
			if (lastInside) {
				//get the vertex on the line
				float x = (lastVertex.x > vertex.x ? clippingAABB.lowerBound.x : clippingAABB.upperBound.x);
				float y = (lastVertex.y > vertex.y ? clippingAABB.lowerBound.y : clippingAABB.upperBound.y);

				b2Vec2 a;
				if (Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(x, clippingAABB.lowerBound.y), b2Vec2(x, clippingAABB.upperBound.y)))
					a = Geometry::GetMidPointByX(x, lastVertex, vertex);
				else if (Geometry::CheckSegmentIntersect(lastVertex, vertex, b2Vec2(clippingAABB.lowerBound.x, y), b2Vec2(clippingAABB.upperBound.x, y)))
					a = Geometry::GetMidPointByY(y, lastVertex, vertex);

				addVertexToContainer(a);

				lastVertex = vertex;
				lastInside = false;
				continue;
			}
			else {
				lastInside = false;

				//we know that both vertices are not inside AABB
				if (Geometry::CheckSegmentIntersect(topLeft, bottomRight, lastVertex, vertex) ||
					Geometry::CheckSegmentIntersect(clippingAABB.lowerBound, clippingAABB.upperBound, lastVertex, vertex)) {
					//intersect with AABB. Now we gonna find those 2 points.
					b2Vec2 p[2] = { lastVertex, vertex }; int i = 0;


					if (Geometry::CheckSegmentIntersect(clippingAABB.lowerBound, topLeft, p[0], p[1])) {
						b2Vec2 a = Geometry::GetMidPointByX(clippingAABB.lowerBound.x, p[0], p[1]);
						if (p[0].x < p[1].x) p[0] = a;
						else p[1] = a;
						i++;
					}
					if (Geometry::CheckSegmentIntersect(bottomRight, clippingAABB.upperBound, p[0], p[1])) {
						b2Vec2 a = Geometry::GetMidPointByX(clippingAABB.upperBound.x, p[0], p[1]);
						if (p[0].x < p[1].x) p[1] = a;
						else p[0] = a;
						i++;
					}

					if (i < 2) {
						if (Geometry::CheckSegmentIntersect(clippingAABB.lowerBound, bottomRight, p[0], p[1])) {
							b2Vec2 a = Geometry::GetMidPointByY(clippingAABB.lowerBound.y, p[0], p[1]);
							if (p[0].y < p[1].y) p[0] = a;
							else p[1] = a;
							i++;
						}
					}
					if (i < 2) {
						if (Geometry::CheckSegmentIntersect(topLeft, clippingAABB.upperBound, p[0], p[1])) {
							b2Vec2 a = Geometry::GetMidPointByY(clippingAABB.upperBound.y, p[0], p[1]);
							if (p[0].y < p[1].y) p[1] = a;
							else p[0] = a;
						}
					}
					addVertexToContainer(p[0]);

					//ignore the vertex
					lastVertex = vertex;
					vertex = p[1];
				}
				else {
					//ignore the vertex
					lastVertex = vertex;
					continue;
				}
			}
		}
		addVertexToContainer(vertex);
	}
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

