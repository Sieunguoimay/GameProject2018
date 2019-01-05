#include"Sprite.h"
#include<SDL2\SDL.h>
Sprite::Sprite(Texture*texture, const glm::ivec2& dimension)
	:m_pTexture(texture),m_dimension(dimension)
{
	m_size.w = texture->GetWidth() / dimension.x;
	m_size.h = texture->GetWidth() / dimension.y;

}
Sprite::~Sprite() {
}
glm::vec4 Sprite::GetUVs(int index) {
	int xTile = index%m_dimension.x;
	int yTile = index/m_dimension.x;
	glm::vec4 uvs;
	uvs.x = (float)xTile / (float)m_dimension.x;
	uvs.y = (float)yTile / (float)m_dimension.y;
	uvs.z = 1.0f / (float)m_dimension.x;
	uvs.w = 1.0f / (float)m_dimension.y;
	return uvs;
}
