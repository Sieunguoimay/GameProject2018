#pragma once
#include"Texture.h"
#include"glm\glm.hpp"
//new sprite for new object
//why reuse this? why points to it?
//One Sprite One texture
class Sprite {
	Texture*m_pTexture;
	glm::ivec2 m_dimension;//tiles
	struct Size { int w, h; }m_size;//pixels
public:
	Sprite(Texture*texture,const glm::ivec2& m_dimension);
	~Sprite();
	glm::vec4 GetUVs(int index);
	Texture*GetTexture() { return this->m_pTexture; }
	glm::ivec2&GetDimension() { return this->m_dimension; }
	Size GetSize() { return this->m_size; }
};
