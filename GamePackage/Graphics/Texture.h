#pragma once
#include"SDL2\SDL_opengles2.h"
#include<string>
class Texture {
	GLuint m_textureId;
	int m_width;
	int m_height;
	std::string m_name;
public:
	Texture();
	~Texture();
	bool Init(const char*path);
	void Bind(GLint unit = 0);
	GLuint GetId() { return this->m_textureId; }

	int GetWidth() { return this->m_width; }
	int GetHeight() { return this->m_height; }
	const std::string&GetName() { return m_name; }
};
