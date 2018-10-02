#include "Mesh.h"
#include"SDL2\SDL.h"
#include"SDL2\SDL_rwops.h"
#include"../misc/Assistances.h"
Mesh::Mesh()
{
	//by default
	m_drawMode = GL_TRIANGLES;
	m_numIndices = 0;

}
bool Mesh::Init(std::string path) {
	
	std::vector<GLfloat>vertices;
	std::vector<GLushort>indices;

	std::string tag = path;
	tag.erase(0,tag.find('.')+1);

	//printf("tag %s\n", tag.c_str());
	if (tag == "obj") {
		if (!LoadObj(path.c_str(), vertices, indices)) {
			return false;
		}
	}
	else if (tag == "nfg") {
		if (!LoadNfg(path.c_str(), vertices, indices)) {
			return false;
		}
	}
	Init(vertices, indices);
	SDL_Log("Model %s Loaded - size %d", path.c_str(), indices.size());
	return true;
}
bool Mesh::Init(std::vector<GLfloat>&vertices, std::vector<GLushort>&indices) {
	m_numIndices = indices.size();

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*indices.size(), &(indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//SDL_Log("size %d %d", vertices.size(),indices.size());


	return true;
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1,&eboId);
}
void Mesh::SetVertexAttribPointer(Shaders * shaders)
{
	GLint pos = shaders->a_posL;
	if (pos != -1) {
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	}
	pos = shaders->a_norm;
	if (pos != -1) {
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	pos = shaders->a_texcoord;
	if (pos != -1) {
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	}

}
void Mesh::Draw(Shaders*shaders) {

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	SetVertexAttribPointer(shaders);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glDrawElements(m_drawMode, m_numIndices, GL_UNSIGNED_SHORT, (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
