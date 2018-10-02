
#include "DebugDraw.h"

#include <stdio.h>
#include <stdarg.h>
#include"../2D/SpriteManager.h"
//#include "Testbed/imgui/imgui.h"
#include"../misc/GameAccessor.h"
#include"../Graphics/ResourceManager.h"
#define BUFFER_OFFSET(x)  ((const void*) (x))

DebugDraw g_debugDraw;
DebugCamera g_DebugCamera;

//
b2Vec2 DebugCamera::ConvertScreenToWorld(const b2Vec2& ps)
{
    float32 w = float32(m_width);
    float32 h = float32(m_height);
	float32 u = ps.x / w;
	float32 v = (h - ps.y) / h;

	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	b2Vec2 pw;
	pw.x = (1.0f - u) * lower.x + u * upper.x;
	pw.y = (1.0f - v) * lower.y + v * upper.y;
	return pw;
}

//
b2Vec2 DebugCamera::ConvertWorldToScreen(const b2Vec2& pw)
{
	float32 w = float32(m_width);
	float32 h = float32(m_height);
	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	float32 u = (pw.x - lower.x) / (upper.x - lower.x);
	float32 v = (pw.y - lower.y) / (upper.y - lower.y);

	b2Vec2 ps;
	ps.x = u * w;
	ps.y = (1.0f - v) * h;
	return ps;
}

// Convert from world coordinates to normalized device coordinates.
// http://www.songho.ca/opengl/gl_projectionmatrix.html
void DebugCamera::BuildProjectionMatrix(float32* m, float32 zBias)
{
	float32 w = float32(m_width);
	float32 h = float32(m_height);
	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	m[0] = 2.0f / (upper.x - lower.x);
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 2.0f / (upper.y - lower.y);
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;

	m[12] = -(upper.x + lower.x) / (upper.x - lower.x);
	m[13] = -(upper.y + lower.y) / (upper.y - lower.y);
	m[14] = zBias;
	m[15] = 1.0f;
}


//
DebugDraw::DebugDraw()
{

}

//
DebugDraw::~DebugDraw()
{

}


//
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	float32 w = float32(s_gameAccessor.GetWidth());
	float32 h = float32(s_gameAccessor.GetHeight());
	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	b2Vec2 m_center(0.0f, 0.0f);
	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;
	//float32 m[16] = { 0.0f };
	glm::mat4 m;
	m[0][0] = 2.0f / (upper.x - lower.x);
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 2.0f / (upper.y - lower.y);
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = -(upper.x + lower.x) / (upper.x - lower.x);
	m[3][1] = -(upper.y + lower.y) / (upper.y - lower.y);
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
	//s_resourceManager.GetShader(4)->UseProgram();
	//s_resourceManager.GetShader(4)->SetUniformMatrix4fv("u_projection", m);


	std::vector<GLfloat>_vertices;
	for (int i = 0; i < vertexCount; i++) {
		_vertices.push_back(vertices[i].x);
		_vertices.push_back(vertices[i].y);
		_vertices.push_back(0.0f);
	
		_vertices.push_back(0.0f);
		_vertices.push_back(0.0f);
		_vertices.push_back(0.0f);
		
		_vertices.push_back(0.0f);
		_vertices.push_back(0.0f);
	}
	std::vector<GLushort>_indices;
	for (int i = 0; i < vertexCount-2; i++) {
		_indices.push_back(i + 1);
		_indices.push_back(i + 2);
		_indices.push_back(0);
	}
	Mesh mesh;
	mesh.Init(_vertices, _indices);
	mesh.Draw(s_resourceManager.GetShader(4));
}

//
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

}

//
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color){

}

//
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
}

//
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{

}

//
void DebugDraw::DrawTransform(const b2Transform& xf)
{

}

//
void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	s_spriteManager.GetSprite("icon")->Draw(0, 0, p.x,p.y,size,size);
}

//
void DebugDraw::DrawString(int x, int y, const char *string, ...)
{

}

//
void DebugDraw::DrawString(const b2Vec2& pw, const char *string, ...)
{

}

//
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{

}

//
void DebugDraw::Flush()
{

}
