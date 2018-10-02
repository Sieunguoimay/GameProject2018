#pragma once
#include<string>
#include<vector>
#include"SDL2\SDL_opengles2.h"
#include"glm\glm.hpp"
#include"Shaders.h"

class Mesh
{
	int m_numIndices;
	GLenum m_drawMode;
	GLuint vboId;
	GLuint eboId;
public:
	Mesh();
	~Mesh();
	bool Init(std::string path);
	bool Init(std::vector<GLfloat>&vertices, std::vector<GLushort>&indices);



	void SetVertexAttribPointer(Shaders*shaders);
	void Draw(Shaders*shaders);

	const GLuint&GetVBOId() { return vboId; }
	const GLuint&GetEBOId() { return eboId; }
};

