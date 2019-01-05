#include "TextureRenderer.h"
#include<algorithm>
#include"glm\glm.hpp"
#include "Renderer.h"


SpriteMesh::SpriteMesh(const glm::vec4 & destRect, glm::vec4 & uvRect, GLuint textureID, float depth, const glm::vec4 & color, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/)
	:textureID(textureID), depth(depth)
{
	float x1 = uvRect.x;
	float x2 = uvRect.x + uvRect.z;
	float y1 = uvRect.y;
	float y2 = uvRect.y + uvRect.w;

	if (horizontal_flip) {
		x1 = uvRect.x + uvRect.z;
		x2 = uvRect.x;
	}
	if (vertical_flip) {
		y1 = uvRect.y + uvRect.w;
		y2 = uvRect.y;
	}

	topLeft.color = color;
	topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	topLeft.setUV(x1, y2);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x, destRect.y);
	bottomLeft.setUV(x1, y1);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
	bottomRight.setUV(x2, y1);

	topRight.color = color;
	topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	topRight.setUV(x2, y2);
}
SpriteMesh::SpriteMesh(const glm::vec4& destRect, glm::vec4& uvRect, GLuint textureID, float depth, const glm::vec4& color, float angle, const glm::vec2& center, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/)
	:textureID(textureID), depth(depth)
{
	glm::vec2 halfDims = center;// (destRect.z / 2.0f, destRect.w / 2.0f);

	glm::vec2 tl(-center.x, destRect.w - center.y);
	glm::vec2 bl(-center.x, -center.y);
	glm::vec2 tr(destRect.z - center.x, destRect.w - center.y);
	glm::vec2 br(destRect.z - center.x, -center.y);

	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;

	float x1 = uvRect.x;
	float x2 = uvRect.x + uvRect.z;
	float y1 = uvRect.y;
	float y2 = uvRect.y + uvRect.w;

	if (horizontal_flip) {
		x1 = uvRect.x + uvRect.z;
		x2 = uvRect.x;
	}
	if (vertical_flip) {
		y1 = uvRect.y + uvRect.w;
		y2 = uvRect.y;
	}

	topLeft.color = color;
	topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
	topLeft.setUV(x1, y2);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
	bottomLeft.setUV(x1, y1);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
	bottomRight.setUV(x2, y1);

	topRight.color = color;
	topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
	topRight.setUV(x2, y2);
}

glm::vec2 SpriteMesh::rotatePoint(glm::vec2 point, float angle)
{
	float x = point.x*cosf(angle) - point.y*sinf(angle);
	float y = point.x*sinf(angle) + point.y*cosf(angle);
	return glm::vec2(x, y);
}

TextureRenderer::TextureRenderer(Shaders*shader, SortType sortType)
	:m_pShader(shader),m_vbo(0)
{

	m_sortType = sortType;
	glGenBuffers(1, &m_vbo);
}


TextureRenderer::~TextureRenderer()
{
	glDeleteBuffers(1, &m_vbo);
}




void TextureRenderer::Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureID, float depth, glm::vec4 color, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/) {
	m_meshes.emplace_back(SpriteMesh(destRect, uvRect, textureID, depth, color, horizontal_flip, vertical_flip));
	m_pMeshes.push_back(&m_meshes[m_meshes.size() - 1]);
}
void TextureRenderer::Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureID, float depth, glm::vec4 color, float angle, glm::vec2 center /*= glm::vec2(0, 0)*/, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/) {
	m_meshes.emplace_back(destRect, uvRect, textureID, depth, color, angle,center, horizontal_flip,vertical_flip);
	m_pMeshes.push_back(&m_meshes[m_meshes.size() - 1]);
}
void TextureRenderer::Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureID, float depth, glm::vec4 color, glm::vec2 direction, glm::vec2 center /*= glm::vec2(0, 0)*/, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/) {
	const glm::vec2 right(1.0f, 0.0f);
	float angle = acosf(glm::dot(right, direction));
	if (direction.y < 0.0f)angle = -angle;
	m_meshes.emplace_back(destRect, uvRect, textureID, depth, color, angle,center, horizontal_flip, vertical_flip);
	m_pMeshes.push_back(&m_meshes[m_meshes.size() - 1]);
}


void TextureRenderer::Render(const float*matrix) {
	sortGlyphs();
	createRenderBatches();

	m_pShader->UseProgram();
	glUniformMatrix4fv(m_pShader->u_ortho, 1, GL_FALSE, matrix);
	for (auto&a : m_renderBatches) {
		glBindTexture(GL_TEXTURE_2D, a.textureID);
		bindVertexArray();
		glDrawArrays(GL_TRIANGLES, a.offset, a.numVertices);
		unbindVertexArray();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	m_pShader->UnuseProgram();


	m_renderBatches.clear();
	m_meshes.clear();
	m_pMeshes.clear();
}

void TextureRenderer::createRenderBatches()
{
	std::vector<Vertex> vertices;
	vertices.resize(m_meshes.size() * 6);

	if (m_meshes.empty()) { return; }

	int offset = 0;
	int cv = 0;//current vertex

	m_renderBatches.emplace_back(RenderBatch(0, 6, m_pMeshes[0]->textureID));
	vertices[cv++] = m_pMeshes[0]->topLeft;
	vertices[cv++] = m_pMeshes[0]->bottomLeft;
	vertices[cv++] = m_pMeshes[0]->bottomRight;
	vertices[cv++] = m_pMeshes[0]->bottomRight;
	vertices[cv++] = m_pMeshes[0]->topRight;
	vertices[cv++] = m_pMeshes[0]->topLeft;
	offset += 6;

	for (int cg = 1; cg < m_pMeshes.size(); cg++) {
		if (m_pMeshes[cg]->textureID != m_pMeshes[cg - 1]->textureID)
			m_renderBatches.emplace_back(RenderBatch(offset, 6, m_pMeshes[cg]->textureID));
		else
			m_renderBatches.back().numVertices += 6;

		vertices[cv++] = m_pMeshes[cg]->topLeft;
		vertices[cv++] = m_pMeshes[cg]->bottomLeft;
		vertices[cv++] = m_pMeshes[cg]->bottomRight;
		vertices[cv++] = m_pMeshes[cg]->bottomRight;
		vertices[cv++] = m_pMeshes[cg]->topRight;
		vertices[cv++] = m_pMeshes[cg]->topLeft;
		offset += 6;
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void TextureRenderer::bindVertexArray() {

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//color attribute pointer
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//uv attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
}

void TextureRenderer::unbindVertexArray()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextureRenderer::sortGlyphs()
{
	switch (m_sortType) {
	case SortType::BACK_TO_FRONT:
		std::stable_sort(m_pMeshes.begin(), m_pMeshes.end(), compareFrontToBack);
		break;
	case SortType::FRONT_TO_BACK:
		std::stable_sort(m_pMeshes.begin(), m_pMeshes.end(), compareBackToFront);
		break;
	case SortType::TEXTURE:
		std::stable_sort(m_pMeshes.begin(), m_pMeshes.end(), compareTexture);
		break;
	}
}

bool TextureRenderer::compareFrontToBack(SpriteMesh * a, SpriteMesh* b)
{
	return (a->depth<b->depth);
}

bool TextureRenderer::compareBackToFront(SpriteMesh * a, SpriteMesh* b)
{
	return (a->depth>b->depth);
}

bool TextureRenderer::compareTexture(SpriteMesh * a, SpriteMesh* b)
{
	return (a->textureID<b->textureID);
}

