#include "NoAnimationSkin.h"
#include"../misc/Locator.h"

NoAnimationSkin::NoAnimationSkin(Texture * texture)
	:m_pTexture(texture)
	, m_size(texture->GetWidth(), texture->GetHeight())
	, m_flip(FlipType::FLIP_NONE)
	, m_center(0.5f,0.5f)
	, m_angle(0)
	, m_pos(0,0)
{}

void NoAnimationSkin::Draw()
{
	//draw with pivot at the center of rectangle and angle in radian 
	glm::vec4 rect = glm::vec4(m_pos - 0.5f*m_size, m_size);
	const SpriteMesh*a=Locator::GetTextureRenderer()->Draw(
		rect,											//rectangle
		glm::vec4(0, 0, 1, 1),							//UVs
		m_pTexture->GetId(), 0,							//id and depth
		glm::vec4(1.0f), m_angle,						//color and angle
		glm::vec2(m_center.x*rect.z, m_center.y*rect.w),
		m_flip==FlipType::HORIZONTAL_FLIP,				//horizontal flip
		m_flip == FlipType::VERTICAL_FLIP);				//vertical flip

	//Handle NULL 
	if (!a) return;

	if (m_aabbFlag) {
		//calculate AABB
		glm::vec2 p[3] = { 
			a->bottomLeft.position,
			a->topRight.position,
			a->bottomRight.position };

		
		glm::vec4 m_AABB(a->topLeft.position, a->topLeft.position);
		for (int i = 0; i < 3; i++) {
			if (m_AABB.x > p[i].x) m_AABB.x = p[i].x;
			else if (m_AABB.z < p[i].x) m_AABB.z = p[i].x;

			if (m_AABB.y > p[i].y) m_AABB.y = p[i].y;
			else if (m_AABB.w < p[i].y) m_AABB.w = p[i].y;
		}
		m_size.x = m_AABB.z - m_AABB.x;
		m_size.y = m_AABB.w - m_AABB.y;

		m_aabbFlag = false;
	}
}
