#include "TextureRenderer.h"
#include "Renderer.h"
#include"../misc/Assistances.h"

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

TextureRenderer::TextureRenderer(Shaders*shader, SortType sortType, glm::vec4*pWindow)
	:m_pShader(shader),m_vbo(0),m_pWindow(pWindow)
{

	m_sortType = sortType;
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_DYNAMIC_DRAW);

}


TextureRenderer::~TextureRenderer()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}



const SpriteMesh*  TextureRenderer::Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureID, float depth, glm::vec4 color, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/) {
	if (m_disabled||!check_AABB_overlap(*m_pWindow, destRect)) return NULL;

	m_meshes.emplace_back(SpriteMesh(destRect, uvRect, textureID, depth, color, horizontal_flip, vertical_flip));
	m_pMeshes.push_back(&m_meshes[m_meshes.size() - 1]);
	return &m_meshes.back();
}
const SpriteMesh*  TextureRenderer::Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureID, float depth, glm::vec4 color, float angle, glm::vec2 center /*= glm::vec2(0, 0)*/, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/) {
	if (m_disabled || !check_AABB_overlap(*m_pWindow, destRect)) return NULL;

	m_meshes.emplace_back(destRect, uvRect, textureID, depth, color, angle,center, horizontal_flip,vertical_flip);
	m_pMeshes.push_back(&m_meshes[m_meshes.size() - 1]);
	return &m_meshes.back();
}
const SpriteMesh*  TextureRenderer::Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureID, float depth, glm::vec4 color, glm::vec2 direction, glm::vec2 center /*= glm::vec2(0, 0)*/, bool horizontal_flip /*= false*/, bool vertical_flip /*= false*/) {
	if (m_disabled || !check_AABB_overlap(*m_pWindow, destRect)) return NULL;

	const glm::vec2 right(1.0f, 0.0f);
	float angle = glm::cos(glm::dot(right, direction));
	if (direction.y < 0.0f)angle = -angle;
	m_meshes.emplace_back(destRect, uvRect, textureID, depth, color, angle,center, horizontal_flip, vertical_flip);
	m_pMeshes.push_back(&m_meshes[m_meshes.size() - 1]);
	return &m_meshes.back();
}

void TextureRenderer::Render(const float*matrix) {
	sortGlyphs();
	createRenderBatches();

	m_pShader->UseProgram();

	glUniformMatrix4fv(m_pShader->u_ortho, 1, GL_FALSE, matrix);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_counter*sizeof(glm::vec4), m_vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indexCounter*sizeof(GLushort), m_indices);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	//position attribute pointer
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)&m_vertices[0]);
	////uv attribute pointer
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(GLfloat), (void*)&m_vertices[0]);


	for (auto&a:m_renderBatches) {
		glBindTexture(GL_TEXTURE_2D, a.textureID);
		//glDrawArrays(GL_TRIANGLES, a.offset, a.numVertices);
		glDrawElements(GL_TRIANGLES,a.numVertices, GL_UNSIGNED_SHORT,(void*)(a.offset*2));
		/*
			BIG LESTION TO REMEMEBER GOES HERE>>>>

			The size of a void* is a platform dependent value. 
			Typically it's value is 4 or 8 for 32 and 64 bit platforms respectively.

			that means, if you give a number representing a index of an array,
			then that index i will be point to 8*i location of the array.

			that's why here when you multiply offset index with 2.
			you get correct location in array. 16*i. because your array
			is GLushort (16bits).

			so. next time. try to pass the correct pointer to the location
			instead of passing the index of that array to a void* pointer.
			which is just 8bit. and your array, of course, size of element
			is arbitrary. The Nhe!!!
		*/
	}
	//glRenderbufferStorage();
	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_pShader->UnuseProgram();
	CleanBuffer();
}

void TextureRenderer::CleanBuffer()
{
	m_meshes.clear();
	m_pMeshes.clear();
}

void TextureRenderer::createRenderBatches()
{
	//std::vector<Vertex> vertices;

	if (m_meshes.empty()) { return; }



	GLuint offset = 0;
	m_counter = 0;
	m_indexCounter = 0;
	if (!m_renderBatches.empty())
		m_renderBatches.clear();

	//m_renderBatches[m_counter++] = { 0, 6, (GLuint)m_pMeshes[0]->textureID };
	m_renderBatches.emplace_back(0, 6, m_pMeshes[0]->textureID);

	//0
	m_indices[m_indexCounter++] = m_counter;
	m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->topLeft.position, m_pMeshes[0]->topLeft.uv);
	//1
	m_indices[m_indexCounter++] = m_counter;
	m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->bottomLeft.position, m_pMeshes[0]->bottomLeft.uv);
	//2
	m_indices[m_indexCounter++] = m_counter;
	m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->bottomRight.position, m_pMeshes[0]->bottomRight.uv);
	//2
	m_indices[m_indexCounter++] = m_counter-1;

	//0
	m_indices[m_indexCounter++] = m_counter - 3;

	//3
	m_indices[m_indexCounter++] = m_counter;
	m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->topRight.position, m_pMeshes[0]->topRight.uv);

	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->topLeft.position, m_pMeshes[0]->topLeft.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->bottomLeft.position, m_pMeshes[0]->bottomLeft.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->bottomRight.position, m_pMeshes[0]->bottomRight.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->bottomRight.position, m_pMeshes[0]->bottomRight.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->topLeft.position, m_pMeshes[0]->topLeft.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[0]->topRight.position, m_pMeshes[0]->topRight.uv);



	offset += 6;

	for (GLuint cg = 1; cg < m_pMeshes.size(); cg++) {
		if (m_pMeshes[cg]->textureID != m_pMeshes[cg - 1]->textureID) {

			m_renderBatches.emplace_back(offset, 6,m_pMeshes[cg]->textureID );
		}
		else {
			m_renderBatches.back().numVertices += 6;
		}


		//0
		m_indices[m_indexCounter++] = m_counter;
		m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->topLeft.position, m_pMeshes[cg]->topLeft.uv);
		//1
		m_indices[m_indexCounter++] = m_counter;
		m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->bottomLeft.position, m_pMeshes[cg]->bottomLeft.uv);
		//2
		m_indices[m_indexCounter++] = m_counter;
		m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->bottomRight.position, m_pMeshes[cg]->bottomRight.uv);
		//2
		m_indices[m_indexCounter++] = m_counter - 1;

		//0
		m_indices[m_indexCounter++] = m_counter-3;

		//3
		m_indices[m_indexCounter++] = m_counter;
		m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->topRight.position, m_pMeshes[cg]->topRight.uv);

		offset += 6;
	}
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->topLeft.position, m_pMeshes[cg]->topLeft.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->bottomLeft.position, m_pMeshes[cg]->bottomLeft.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->bottomRight.position, m_pMeshes[cg]->bottomRight.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->bottomRight.position, m_pMeshes[cg]->bottomRight.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->topLeft.position, m_pMeshes[cg]->topLeft.uv);
	//m_vertices[m_counter++] = glm::vec4(m_pMeshes[cg]->topRight.position, m_pMeshes[cg]->topRight.uv);

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
	return (a->textureID>b->textureID);
}




//void TextureRenderer::bindVertexArray() {
//
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);
//
//	//position attribute pointer
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
//	//color attribute pointer
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
//
//	//uv attribute pointer
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
//}


