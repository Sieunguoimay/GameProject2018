#pragma once
#include<vector>
#include"Renderer.h"
#include"Vertex.h"
class PrimitiveRenderer:public Renderer
{
	struct PrimitiveVertex {
		glm::vec2 position;
		glm::vec4 color;
	};
	std::vector<PrimitiveVertex>m_vertices;

	std::vector<GLuint>m_drawingIndices;
	int m_numDrawingElements = 0;

	std::vector<GLuint>m_fillingIndices;
	int m_numFillingElements = 0;

	std::vector<GLuint>m_lightIndices;
	int m_numLightElements = 0;

	GLuint m_vbo, m_ibo, m_ibo2, m_ibo3;
	void bindVertexArray();
	void unbindVertexArray();
	void sendBufferData();
	glm::vec2 rotatePoint(glm::vec2 point, float angle);
	void fillCirlce(std::vector<GLuint>&container,const glm::vec2& position, float radius, const glm::vec4& centerColor, const glm::vec4& boundaryColor);

	Shaders*m_pDrawingShader;
	Shaders*m_pFillingShader;
	Shaders*m_pLightShader;

public:
	PrimitiveRenderer(Shaders*drawingShader, Shaders*fillingShader, Shaders*lightShader);
	~PrimitiveRenderer();
	void CreateBox(const glm::vec4& destRect, float angle, const glm::vec2& center,const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawBox(const glm::vec4& destRect, float angle = 0,const glm::vec2& center = glm::vec2(0.0,0.0),const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void FillBox(const glm::vec4& destRect, float angle = 0,const glm::vec2& center = glm::vec2(0.0,0.0), const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	
	void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void FillCircle(const glm::vec2& position, float radius, const glm::vec4& centerColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec4& boundaryColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawLight(const glm::vec2& position, float radius, const glm::vec4& centerColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec4& boundaryColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& boundaryColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void Render(const float*matrix);
};
#include"Box2D\Box2D.h"
class Box2DRenderer : public b2Draw
{
public:

	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	/// Draw a solid circle.
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	/// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf);

	/// Draw a point.
	virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
};
