#include "PrimitiveRenderer.h"



PrimitiveRenderer::PrimitiveRenderer(Shaders*drawingShader, Shaders*fillingShader, Shaders*lightShader)
	: m_pDrawingShader(drawingShader)
	,m_pFillingShader(fillingShader)
	,m_pLightShader(lightShader)
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_ibo2);
	glGenBuffers(1, &m_ibo3);
}


PrimitiveRenderer::~PrimitiveRenderer()
{
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_ibo) glDeleteBuffers(1, &m_ibo);
	if (m_ibo2) glDeleteBuffers(1, &m_ibo2);
	if (m_ibo3) glDeleteBuffers(1, &m_ibo3);
	m_pDrawingShader = NULL;
	m_pFillingShader = NULL;
}

void PrimitiveRenderer::CreateBox(const glm::vec4& destRect, float angle, const glm::vec2& center, const glm::vec4& color) {

	int start = m_vertices.size();
	m_vertices.reserve(start + 4);

	glm::vec2 halfDims = center;/* (destRect.z / 2.0f, destRect.w / 2.0f);*/
	glm::vec2 position(destRect.x, destRect.y);

	glm::vec2 tl(-center.x, destRect.w-center.y);
	glm::vec2 bl(-center.x, -center.y);
	glm::vec2 tr(destRect.z - center.x, destRect.w - center.y);
	glm::vec2 br(destRect.z - center.x, -center.y);

	PrimitiveVertex newVertex;

	newVertex.color = color;
	newVertex.position = rotatePoint(tl, angle) + halfDims + position;
	m_vertices.push_back(newVertex);

	PrimitiveVertex newVertex2;
	newVertex2.color = color;
	newVertex2.position = rotatePoint(bl, angle) + halfDims + position;
	m_vertices.push_back(newVertex2);

	PrimitiveVertex newVertex3;
	newVertex3.color = color;
	newVertex3.position = rotatePoint(tr, angle) + halfDims + position;
	m_vertices.push_back(newVertex3);

	PrimitiveVertex newVertex4;
	newVertex4.color = color;
	newVertex4.position = rotatePoint(br, angle) + halfDims + position;
	m_vertices.push_back(newVertex4);

}

void PrimitiveRenderer::DrawBox(const glm::vec4 & destRect, float angle, const glm::vec2& center, const glm::vec4 & color)
{
	if (m_disabled) return;

	CreateBox(destRect, angle, center,color);
	int start = m_vertices.size() - 4;

	m_drawingIndices.reserve(m_drawingIndices.size() + 8);

	m_drawingIndices.push_back(start);
	m_drawingIndices.push_back(start + 1);

	m_drawingIndices.push_back(start + 1);
	m_drawingIndices.push_back(start + 3);

	m_drawingIndices.push_back(start + 3);
	m_drawingIndices.push_back(start + 2);

	m_drawingIndices.push_back(start + 2);
	m_drawingIndices.push_back(start);
}

void PrimitiveRenderer::FillBox(const glm::vec4 & destRect, float angle, const glm::vec2& center, const glm::vec4 & color)
{
	if (m_disabled) return;

	CreateBox(destRect, angle,center, color);
	int start = m_vertices.size() - 4;

	m_fillingIndices.reserve(m_fillingIndices.size() + 6);

	m_fillingIndices.push_back(start);
	m_fillingIndices.push_back(start + 1);
	m_fillingIndices.push_back(start + 2);

	m_fillingIndices.push_back(start + 2);
	m_fillingIndices.push_back(start + 1);
	m_fillingIndices.push_back(start + 3);
}


void PrimitiveRenderer::DrawCircle(const glm::vec2& position, float radius, const glm::vec4&color) {

	if (m_disabled) return;

	static const int NUM_POINTS = 100;
	int start = m_vertices.size();
	for (int i = 0; i < NUM_POINTS - 1; i++) {
		PrimitiveVertex vertex;
		vertex.position.x = radius*cosf(2.0f*3.141592645f*float(i) / NUM_POINTS) + position.x;
		vertex.position.y = radius*sinf(2.0f*3.141592645f*float(i) / NUM_POINTS) + position.y;
		vertex.color = color;
		m_vertices.push_back(vertex);

		m_drawingIndices.push_back(start + i);
		if (i < NUM_POINTS - 2)
			m_drawingIndices.push_back(start + i + 1);
	}
	m_drawingIndices.push_back(start);
}

void PrimitiveRenderer::FillCircle(const glm::vec2 & position, float radius, const glm::vec4 & centerColor, const glm::vec4 & boundaryColor)
{
	fillCirlce(m_fillingIndices, position, radius, centerColor, boundaryColor);
}

void PrimitiveRenderer::DrawLight(const glm::vec2 & position, float radius, const glm::vec4 & centerColor, const glm::vec4 & boundaryColor)
{
	fillCirlce(m_lightIndices, position, radius, centerColor, boundaryColor);
}

void PrimitiveRenderer::fillCirlce(std::vector<GLuint>&container,const glm::vec2 & position, float radius, const glm::vec4 & centerColor, const glm::vec4 & boundaryColor)
{
	if (m_disabled) return;

	float perimeter = 2.0f*radius*3.141592645f;
	float step = radius / 10.0f;
	int NUM_POINTS = (int)(perimeter / step);
	int start = m_vertices.size();

	PrimitiveVertex vertex;
	vertex.position = position;
	vertex.color = centerColor;
	m_vertices.push_back(vertex);


	for (int i = 0; i < NUM_POINTS; i++) {
		PrimitiveVertex vertex;
		vertex.position.x = radius*cosf(2.0f*3.141592645f*float(i) / NUM_POINTS) + position.x;
		vertex.position.y = radius*sinf(2.0f*3.141592645f*float(i) / NUM_POINTS) + position.y;
		vertex.color = boundaryColor;
		m_vertices.push_back(vertex);

		container.push_back(start);
		container.push_back(start + i + 1);
		if (i < NUM_POINTS - 1)
			container.push_back(start + i + 2);
	}
	container.push_back(start + 1);
}


void PrimitiveRenderer::DrawLine(const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec4 & color)
{
	if (m_disabled) return;

	int start = m_vertices.size();

	PrimitiveVertex newVertex;
	newVertex.color = color;
	newVertex.position = p1;
	m_vertices.push_back(newVertex);

	PrimitiveVertex newVertex2;
	newVertex2.color = color;
	newVertex2.position = p2;
	m_vertices.push_back(newVertex2);

	m_drawingIndices.push_back(start);
	m_drawingIndices.push_back(start+1);
}

void PrimitiveRenderer::Render(const float*matrix)
{
	if (m_disabled) return;

	sendBufferData();
	if (m_numDrawingElements > 0) {
		m_pDrawingShader->UseProgram();
		glUniformMatrix4fv(m_pDrawingShader->u_ortho, 1, GL_FALSE, matrix);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		bindVertexArray();
		glLineWidth(3);
		glDrawElements(GL_LINES, m_numDrawingElements, GL_UNSIGNED_INT, 0);
		unbindVertexArray();
		m_pDrawingShader->UnuseProgram();
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	if (m_numFillingElements > 0) {
		m_pFillingShader->UseProgram();
		glUniformMatrix4fv(m_pFillingShader->u_ortho, 1, GL_FALSE, matrix);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo2);
		bindVertexArray();
		glDrawElements(GL_TRIANGLES, m_numFillingElements, GL_UNSIGNED_INT, 0);
		unbindVertexArray();
		m_pFillingShader->UnuseProgram();
	}
	if (m_numLightElements > 0) {
		m_pLightShader->UseProgram();
		glUniformMatrix4fv(m_pLightShader->u_ortho, 1, GL_FALSE, matrix);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo3);
		bindVertexArray();
		glDrawElements(GL_TRIANGLES, m_numLightElements, GL_UNSIGNED_INT, 0);
		unbindVertexArray();
		m_pLightShader->UnuseProgram();
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void PrimitiveRenderer::bindVertexArray() {

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//position attribute pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), (void*)offsetof(PrimitiveVertex, position));
	//color attribute pointer
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), (void*)offsetof(PrimitiveVertex, color));
}

void PrimitiveRenderer::unbindVertexArray()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void PrimitiveRenderer::sendBufferData()
{
	if (m_vertices.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(PrimitiveVertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size()*sizeof(PrimitiveVertex), m_vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_vertices.clear();
	}

	m_numDrawingElements = m_drawingIndices.size();
	if (m_numDrawingElements > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_drawingIndices.size()*sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_drawingIndices.size()*sizeof(GLuint), m_drawingIndices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_drawingIndices.clear();
	}
	m_numFillingElements = m_fillingIndices.size();
	if (m_numFillingElements > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_fillingIndices.size()*sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_fillingIndices.size()*sizeof(GLuint), m_fillingIndices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_fillingIndices.clear();
	}
	m_numLightElements = m_lightIndices.size();
	if (m_numLightElements > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo3);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_lightIndices.size()*sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_lightIndices.size()*sizeof(GLuint), m_lightIndices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_lightIndices.clear();
	}

}
glm::vec2 PrimitiveRenderer::rotatePoint(glm::vec2 point, float angle)
{
	float x = point.x*cosf(angle) - point.y*sinf(angle);
	float y = point.x*sinf(angle) + point.y*cosf(angle);
	return glm::vec2(x, y);
}

#include"../misc/Locator.h"
#include"../misc/Assistances.h"
void Box2DRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	for (int i = 0; i < vertexCount-1; i++) {
		glm::vec2 p1(vertices[i].x, vertices[i].y);
		glm::vec2 p2(vertices[i+1].x, vertices[i+1].y);
		glm::vec4 _color(color.r, color.g, color.b, color.a);
		Locator::GetPrimitiveRenderer()->DrawLine(p1*M2P, p2*M2P,_color);
	}
	glm::vec2 p1(vertices[vertexCount - 1].x, vertices[vertexCount - 1].y);
	glm::vec2 p2(vertices[0].x, vertices[0].y);
	glm::vec4 _color(color.r, color.g, color.b, color.a);
	Locator::GetPrimitiveRenderer()->DrawLine(p1*M2P, p2*M2P, _color);
}

//
void Box2DRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i < vertexCount-1 ; i++) {
		glm::vec2 p1(vertices[i].x, vertices[i].y);
		glm::vec2 p2(vertices[i + 1].x, vertices[i + 1].y);
		glm::vec4 _color(color.r, color.g, color.b, color.a);
		Locator::GetPrimitiveRenderer()->DrawLine(p1*M2P, p2*M2P, _color);
	}
	glm::vec2 p1(vertices[vertexCount - 1].x, vertices[vertexCount - 1].y);
	glm::vec2 p2(vertices[0].x, vertices[0].y);
	glm::vec4 _color(color.r, color.g, color.b, color.a);
	Locator::GetPrimitiveRenderer()->DrawLine(p1*M2P, p2*M2P, _color);

}

//
void Box2DRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
	glm::vec4 _color(color.r, color.g, color.b, color.a);
	glm::vec2 pos(center.x, center.y);
	Locator::GetPrimitiveRenderer()->DrawCircle(pos*M2P,radius*M2P, _color);
}

//
void Box2DRenderer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	glm::vec4 _color(color.r, color.g, color.b, color.a);
	glm::vec2 pos(center.x, center.y);
	Locator::GetPrimitiveRenderer()->DrawCircle(pos*M2P, radius*M2P, _color);
}

//
void Box2DRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glm::vec4 _color(color.r, color.g, color.b, color.a);
	Locator::GetPrimitiveRenderer()->DrawLine(glm::vec2( p1.x,p1.y)*M2P, glm::vec2(p2.x,p2.y)*M2P, _color);
}

//
void Box2DRenderer::DrawTransform(const b2Transform& xf)
{

}

//
void Box2DRenderer::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glm::vec4 _color(color.r, color.g, color.b, color.a);
	Locator::GetPrimitiveRenderer()->DrawBox(glm::vec4(p.x, p.y,2,2)*M2P,0,glm::vec2(1,1), _color);
}
