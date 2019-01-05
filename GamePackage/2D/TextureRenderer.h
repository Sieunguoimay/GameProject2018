#pragma once
#include<vector>
#include<SDL2\SDL_opengles2.h>
#include"Renderer.h"
#include"Shaders.h"
#include"Vertex.h"
#include"glm\glm.hpp"
enum SortType {
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};
class SpriteMesh {
public:
	SpriteMesh(const glm::vec4& destRect, glm::vec4& uvRect, GLuint textureID, float depth, const glm::vec4& color, bool horizonal_flip = false, bool vertical_flip = false);
	SpriteMesh(const glm::vec4& destRect, glm::vec4& uvRect, GLuint textureID, float depth, const glm::vec4& color, float angle, const glm::vec2& center , bool horizonal_flip = false, bool vertical_flip = false);
	GLuint textureID;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
private:
	glm::vec2 rotatePoint(glm::vec2 point, float angle);
};
class RenderBatch {
public:
	RenderBatch(GLuint offset, GLuint numVertices, GLuint textureID)
		:offset(offset), numVertices(numVertices), textureID(textureID)
	{}
	GLuint offset;
	GLuint numVertices;
	GLuint textureID;
};

//NOTE: limit 1500 draw calls per frame. why?
class TextureRenderer:public Renderer
{
public:
	TextureRenderer(Shaders*shader,SortType sortType = SortType::FRONT_TO_BACK);
	virtual ~TextureRenderer() override;

	void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, bool horizontal_flip = false, bool vertical_flip = false);
	void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, float angle, glm::vec2 center = glm::vec2(0, 0), bool horizontal_flip = false,bool vertical_flip = false);
	void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, glm::vec2 direction, glm::vec2 center = glm::vec2(0, 0), bool horizontal_flip = false, bool vertical_flip = false);

	virtual void Render(const float*matrix) override;

private:
	void createRenderBatches();
	void bindVertexArray();
	void unbindVertexArray();
	void sortGlyphs();
	static bool compareFrontToBack(SpriteMesh*a, SpriteMesh*b);
	static bool compareBackToFront(SpriteMesh*a, SpriteMesh*b);
	static bool compareTexture(SpriteMesh*a, SpriteMesh*b);
	SortType m_sortType;
	GLuint m_vbo;
	std::vector<SpriteMesh*>m_pMeshes;
	std::vector<SpriteMesh>m_meshes;
	std::vector<RenderBatch>m_renderBatches;
	Shaders*m_pShader;
};

