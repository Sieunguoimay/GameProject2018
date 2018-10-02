#include "Texture.h"
#include"SDL2\SDL.h"
#include"SDL2\SDL_image.h"
#include"../misc/Assistances.h"
Texture::Texture()
	:m_textureId(0)
{
	
}
bool Texture::Init(const char*path) {
	int startTime = SDL_GetTicks();
	
	SDL_Surface*image = IMG_Load(path);
	GLenum format;
	if (image->format->BytesPerPixel == 3)
		format = GL_RGB;
	else if (image->format->BytesPerPixel == 4)
		format = GL_RGBA;

	m_width = image->w;
	m_height = image->h;
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
	SDL_FreeSurface(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_name = path;
	m_name.erase(0, m_name.find_last_of('/')+1);
	m_name.erase(m_name.find_last_of('.'), m_name.size());
	SDL_Log("Time of Loading Texture %f ", (float)(SDL_GetTicks() - startTime) / 1000.0f);
	return true;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureId);
}

void Texture::Bind(GLint unit)
{
	if(unit>0)
		glActiveTexture(GL_TEXTURE0 + unit);

	glBindTexture(GL_TEXTURE_2D,m_textureId);
}
