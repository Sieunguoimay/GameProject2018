#pragma once
#include<SDL2\SDL_opengles2.h>
#include<string>
#include<glm\glm.hpp>
class Texture {
	GLuint m_textureId;
	glm::ivec2 m_dimension;
	std::string m_name; 
public:
	Texture();
	~Texture();
	bool Init(const char*path);
	void Bind(GLint unit = 0);
	void Unbind();
	GLuint GetId() { return this->m_textureId; }

	int GetWidth() { return this->m_dimension.x; }
	int GetHeight() { return this->m_dimension.y; }
	glm::ivec2&GetDimension() { return m_dimension; }

	const std::string&GetName() { return m_name; }
	void SetName(std::string name) { this->m_name = name; }
};
