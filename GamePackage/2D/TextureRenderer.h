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
struct RenderBatch {
//public:
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

	virtual void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, bool horizontal_flip = false, bool vertical_flip = false);
	virtual void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, float angle, glm::vec2 center = glm::vec2(0, 0), bool horizontal_flip = false,bool vertical_flip = false);
	virtual void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, glm::vec2 direction, glm::vec2 center = glm::vec2(0, 0), bool horizontal_flip = false, bool vertical_flip = false);

	virtual void Render(const float*matrix) override;
	int GetDrawNum() { return m_renderBatches.size(); }
protected:
	void createRenderBatches();
	void sortGlyphs();
	static bool compareFrontToBack(SpriteMesh*a, SpriteMesh*b);
	static bool compareBackToFront(SpriteMesh*a, SpriteMesh*b);
	static bool compareTexture(SpriteMesh*a, SpriteMesh*b);



	SortType m_sortType;
	GLuint m_vbo;
	GLuint m_ibo;

	enum { e_maxVertices = 4096*2 };//maximum restricted by m_counter (unsigned short - 16) 2^16=65536
	glm::vec4 m_vertices[e_maxVertices];//4 vertices per quad (glm::vec4)
	GLushort m_indices[3*e_maxVertices/2];//6 indices per quad (GLushort) 
	GLushort m_counter;//65536
	GLuint m_indexCounter;//only reachs to on this program 3*2^16 < 2^32=2^16*2^16

	std::vector<SpriteMesh*>m_pMeshes;
	std::vector<SpriteMesh>m_meshes;
	std::vector<RenderBatch>m_renderBatches;
	//RenderBatch m_renderBatches[200];
	Shaders*m_pShader;
};




















class TextureRendererTest :public TextureRenderer {
	glm::mat4 m_camMatrix;
	GLuint m_vbo;
public:
	TextureRendererTest(Shaders*shader, SortType sortType = SortType::FRONT_TO_BACK);
	~TextureRendererTest() override;

	void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, bool horizontal_flip = false, bool vertical_flip = false);
	void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, float angle, glm::vec2 center = glm::vec2(0, 0), bool horizontal_flip = false, bool vertical_flip = false);
	void Draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint textureId, float depth, glm::vec4 color, glm::vec2 direction, glm::vec2 center = glm::vec2(0, 0), bool horizontal_flip = false, bool vertical_flip = false);

	void Draw(glm::vec4 destRect, GLuint textureId, float angle);

	void Render(const float*matrix) override;

};
